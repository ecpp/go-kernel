
#define _CRT_SECURE_NO_WARNINGS
#include "globals.h"
#include "src/utils/vector.h"
#include <atomic>
#include <thread>
#include <iostream>
#include "src/hacks/hacks.h"
#include "src/utils/driver.h"
#include "src/utils/auto_updater.hpp"
#include "src/gui/gui_esp.h"
#include "src/utils/obfuscate.h"
#include "src/utils/nondrivermem.h"
#include <codecvt>
#include <locale>



bool readUserToken() {
	char* appDataDir = nullptr;
	size_t len;

	errno_t err = _dupenv_s(&appDataDir, &len, "APPDATA");
	if (err != 0 || appDataDir == nullptr) {
		return false;
	}

	std::string appDataPath = appDataDir;
	std::string filePath = appDataPath + "\\gokernel.txt";

	std::ifstream file(filePath);
	if (!file.good()) {
		return false;
	}

	std::string line;
	std::getline(file, line);

	globals::userToken = line;

	return true;
}


int main()
{
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	/*if (!readUserToken()) {
		std::cout << "Failed to read user token." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 0;
	}
	
	std::cout << "Version check ......";

	if (!globals::api.checkVersion("csgo")) {
		std::cout << RED << "FAILED!" << RESET << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 0;
	}

	std::cout << BOLDGREEN << "OK" << RESET << std::endl;

	HW_PROFILE_INFO hwProfileInfo;
	if (GetCurrentHwProfile(&hwProfileInfo)) {
		globals::hwid = hwProfileInfo.szHwProfileGuid;
	}
	else {
		std::cout << "Failed to get HWID." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 0;
	}

	std::cout << "License check ......";

	if (!globals::api.validateProduct(1)) {
		std::cout << RED << "FAILED!" << RESET << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 0;
	}*/

	std::cout << BOLDGREEN << "OK" << RESET << std::endl;
	
	/*std::cout << "Driver check ......";

	if (!globals::api.downloadDriver(1, 1)) {
		std::cout << RED << "FAILED!" << RESET << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 0;
	}*/

	std::this_thread::sleep_for(std::chrono::seconds(2));

	std::cout << BOLDGREEN << "OK" << RESET << std::endl;

	globals::processID = Driver::get_process_id(AY_OBFUSCATE("cs2.exe"));

	while (!globals::processID) {
		std::cout << "Either csgo.exe is not open or driver failed to load." << std::endl;
		globals::processID = Driver::get_process_id(AY_OBFUSCATE("cs2.exe"));
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	std::cout << "Kernel Driver ......" << BOLDGREEN << "LOADED!" << RESET << std::endl;

	globals::client = Driver::get_client(globals::processID);
	globals::clientsize	= Driver::get_client_size(globals::processID);
	/*globals::engine = Driver::get_engine(globals::processID);
	globals::enginesize = Driver::get_engine_size(globals::processID);*/


	if (!globals::client) {
		std::cout << "Failed to find client.dll..." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		return 0;
	}


	updater::check_and_update(true);


	std::cout << "G0-K3RN3L ......" << BOLDGREEN << "STARTED!" << RESET << std::endl;

	/*std::thread(hax::checkGameWindow).detach();
	std::thread(gui_menu::startMenu).detach();
	std::thread(hax::readGlobals).detach();
	std::thread(hax::bunny).detach();
	std::thread(hax::radar).detach();
	std::thread(hax::legitAim).detach();
	std::thread(hax::noRecoil).detach();
	std::thread(hax::trigger).detach();
	std::thread(gui_esp::drawAll).detach();
	std::thread(hax::noFlash).detach();
	std::thread(hax::autoAccept).detach();*/
	std::thread(gui_menu::startMenu).detach();
	std::thread(gui_esp::drawAll).detach();
	//std::cout << AY_OBFUSCATE("\033[1;31mPress \033[1;32mINSERT\033[1;31m to open in game menu\033[0m") << std::endl;

	while (true) {
		
		globals::client = Driver::get_client(globals::processID);
		globals::clientsize = Driver::get_client_size(globals::processID);
		
		std::this_thread::sleep_for(std::chrono::seconds(30));
	}

	return 0;
}

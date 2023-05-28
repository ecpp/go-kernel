
#define _CRT_SECURE_NO_WARNINGS
#include "globals.h"
#include "src/utils/vector.h"
#include <atomic>
#include <thread>
#include <iostream>
#include "src/hacks/hacks.h"
#include "src/utils/driver.h"
#include "src/utils/loader.h"
#include "src/gui/gui_esp.h"
#include "src/utils/obfuscate.h"
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
	
	if (!readUserToken()) {
		std::cout << AY_OBFUSCATE("Failed to read user token.") << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 0;
	}
	
	std::cout << AY_OBFUSCATE("Version check ......");

	if (!globals::api.checkVersion("csgo")) {
		std::cout << RED << AY_OBFUSCATE("FAILED!") << RESET << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 0;
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << GREEN << AY_OBFUSCATE("OK") << RESET << std::endl;

	HW_PROFILE_INFO hwProfileInfo;
	if (GetCurrentHwProfile(&hwProfileInfo)) {
		globals::hwid = hwProfileInfo.szHwProfileGuid;
	}
	else {
		std::cout << AY_OBFUSCATE("Failed to get HWID.") << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 0;
	}

	std::cout << AY_OBFUSCATE("License check ......");

	if (!globals::api.validateProduct(1)) {
		std::cout << RED << AY_OBFUSCATE("FAILED!") << RESET << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 0;
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << GREEN << AY_OBFUSCATE("OK") << RESET << std::endl;
	
	std::cout << AY_OBFUSCATE("Driver check ......");

	if (!globals::api.downloadDriver(1, 1)) {
		std::cout << RED << AY_OBFUSCATE("FAILED!") << RESET << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 0;
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << GREEN << AY_OBFUSCATE("OK") << RESET << std::endl;

	globals::processID = Driver::get_process_id(AY_OBFUSCATE("csgo.exe"));

	while (!globals::processID) {
		std::cout << AY_OBFUSCATE("Either csgo.exe is not open or driver failed to load.") << std::endl;
		globals::processID = Driver::get_process_id(AY_OBFUSCATE("csgo.exe"));
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	std::cout << AY_OBFUSCATE("Kernel Driver ......") << GREEN << AY_OBFUSCATE("LOADED!") << RESET << std::endl;

	globals::client = Driver::get_client(globals::processID);
	globals::engine = Driver::get_engine(globals::processID);

	if (!globals::client || !globals::engine) {
		std::cout << AY_OBFUSCATE("Failed to find client.dll or engine.dll") << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		return 0;
	}

	std::cout << AY_OBFUSCATE("G0-K3RN3L ......") << GREEN << AY_OBFUSCATE("STARTED!") << RESET << std::endl;



	std::thread(gui_menu::startMenu).detach();
	std::thread(hax::readGlobals).detach();
	std::thread(hax::bunny).detach();
	std::thread(hax::radar).detach();
	std::thread(hax::legitAim).detach();
	std::thread(hax::noRecoil).detach();
	std::thread(hax::trigger).detach();
	std::thread(gui_esp::drawAll).detach();
	std::thread(hax::noFlash).detach();
	std::thread(hax::autoAccept).detach();
	std::cout << AY_OBFUSCATE("\033[1;31mPress \033[1;32mINSERT\033[1;31m to open in game menu\033[0m") << std::endl;


	while (true) {
		//check if csgo running
		if (!Driver::get_process_id(AY_OBFUSCATE("csgo.exe"))) {
			std::cout << AY_OBFUSCATE("csgo.exe is not running, exiting...") << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
			//exit process
			ExitProcess(0);
			return 0;
		}
		std::this_thread::sleep_for(std::chrono::minutes(2));
	}

	return 0;
}

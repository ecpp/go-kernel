
#define _CRT_SECURE_NO_WARNINGS
#include "src/utils/memory.h"
#include "src/utils/vector.h"
#include <atomic>
#include <thread>
#include <iostream>
#include "src/hacks/hacks.h"
#include "globals.h"
#include "src/utils/driver.h"
#include "src/utils/loader.h"
#include "src/gui/gui_esp.h"
#include "src/utils/obfuscate.h"

int main()
{

	loader();


	globals::processID = Driver::get_process_id("csgo.exe");
	
	while (!globals::processID) {
		std::cout << "Either csgo.exe is not open or driver failed to load." << std::endl;
		globals::processID = Driver::get_process_id("csgo.exe");
	}

	std::cout << RED << "Kernel Driver" << RESET << " loaded successfully!" << std::endl;

	globals::client = Driver::get_client(globals::processID);
	globals::engine = Driver::get_engine(globals::processID);

	std::cout << BLUE << "ID (csgo.exe):" << GREEN << globals::processID << std::endl;


	if (!globals::client || !globals::engine) {
		std::cout << "Failed to find client.dll or engine.dll" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		return 0;
	}

	std::cout << BLUE << "ID (client.dll):" << GREEN << globals::client << std::endl;
	std::cout << BLUE << "ID (engine.dll):" << GREEN << globals::engine << std::endl;

	

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
	std::cout << "\033[1;31mPress \033[1;32mINSERT\033[1;31m to open in game menu\033[0m" << std::endl;


	while (true) {
		//check if csgo running
		if (!Driver::get_process_id("csgo.exe")) {
			std::cout << "csgo.exe is not running, exiting..." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
			//exit process
			ExitProcess(0);
			return 0;
		}
		std::this_thread::sleep_for(std::chrono::minutes(10));
	}

	return 0;
}

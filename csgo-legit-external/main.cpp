#include "src/utils/memory.h"
#include "src/utils/vector.h"
#include <atomic>
#include <thread>
#include <iostream>
#include "src/hacks/hacks.h"
#include "globals.h"
#include "src/utils/driver.h"
#include "src/utils/loader.h"

int main()
{
	loader();


	globals::processID = Driver::get_process_id("csgo.exe");
	
	while (!globals::processID) {
		globals::processID = Driver::get_process_id("csgo.exe");
	}

	globals::client = Driver::get_client(globals::processID);
	globals::engine = Driver::get_engine(globals::processID);

	std::cout << "ID (csgo.exe):" << globals::processID << std::endl;


	if (!globals::client || !globals::engine) {
		std::cout << "Failed to find client.dll or engine.dll" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		return 0;
	}

	std::cout << "ID (client.dll):" << globals::client << std::endl;
	std::cout << "ID (engine.dll):" << globals::engine << std::endl;

	std::thread(hax::readGlobals).detach();
	std::thread(hax::aim).detach();
	std::thread(hax::bunny).detach();
	std::thread(hax::radar).detach();
	std::thread(hax::legitAim).detach();
	std::thread(hax::noRecoil).detach();
	std::thread(hax::trigger).detach();
	std::thread(hax::esp).detach();

	std::thread getUserInput([&]() {
		while (globals::runhax) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			std::cout << "1) Rage Aim Status: " << globals::isaim << std::endl;
			std::cout << "2) Legit Aim Status: " << globals::islegitAim << std::endl;
			std::cout << "3) Radar Status: " << globals::isradar << std::endl;
			std::cout << "4) Bhop Status: " << globals::isbunny << std::endl;
			std::cout << "5) No Recoil Status: " << globals::isNoRecoil<< std::endl;
			std::cout << "6) Trigger Status: " << globals::isTrigger << std::endl;
			std::cout << "7) ESP Status: " << globals::isEsp << std::endl;
			std::cout << "8) Exit. " << std::endl;
			std::cout << "Enter the number you want to activate: ";
			std::string input;
			std::cin >> input;
			if (input == "1") {
				globals::islegitAim = false;
				globals::isaim = !globals::isaim;

			}
			else if (input == "2") {
				globals::isaim = false;
				globals::islegitAim = !globals::islegitAim;
			}
			else if (input == "3") {
				globals::isradar = !globals::isradar;
			}
			else if (input == "4") {
				globals::isbunny = !globals::isbunny;
			}
			else if (input == "5") {
				globals::isNoRecoil = !globals::isNoRecoil;
			}
			else if (input == "6") {
				globals::isTrigger = !globals::isTrigger;
			}
			else if (input == "7") {
				globals::isEsp = !globals::isEsp;
				//Driver::setEsp(true);
			}
			else {
				std::cout << "Exiting..." << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				globals::runhax = false;
				return 0;
			}
			system("CLS");

		}
		});

	getUserInput.join();

	return 0;
}

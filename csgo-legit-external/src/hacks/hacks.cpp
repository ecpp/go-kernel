#include "../../globals.h"
#include "hacks.h"
#include <thread>
#include <iostream>
#include "../utils/vector.h"
#include "../utils/driver.h"

#include "../gui/gui_esp.h"
#include "../gui/gui_menu.h"

constexpr Vector3 CalculateAngle(
	const Vector3& localPosition,
	const Vector3& enemyPosition,
	const Vector3& viewAngles) noexcept
{
	return ((enemyPosition - localPosition).ToAngle() - viewAngles);
}


//void hax::readGlobals() noexcept
//{
//	while (globals::runhax) {
//		
//
//		globals::localPlayer = Driver::rpm<std::uint32_t>(globals::client + offset::dwLocalPlayerPawn);
//		globals::localTeam = Driver::rpm<std::int32_t>(globals::localPlayer + offset::m_iTeamNum);
//		globals::localPlayerFlags = Driver::rpm<std::uint32_t>(globals::localPlayer + offset::m_fFlags);
//		globals::clientState = Driver::rpm<std::uint32_t>(globals::engine + offset::dwClientState);
//		globals::mapDirectory = Driver::rpm<std::array<char, 0x80>>(globals::clientState + offset::dwClientState_MapDirectory);
//		globals::gameDirectory = Driver::rpm<std::array<char, 0x100>>(globals::engine + offset::dwGameDir);
//		globals::mapName = Driver::rpm<std::array<char, 0x120>>(globals::clientState + offset::dwClientState_Map);
//		globals::fullPath = std::string(globals::gameDirectory.data()) + '\\' + std::string(globals::mapDirectory.data());
//		std::this_thread::sleep_for(std::chrono::seconds(10));
//	}
//	
//}

void hax::checkGameWindow() noexcept
{
	while (globals::runhax) {
		auto hwnd = FindWindow(0, _T(globals::windowName));
		if (hwnd == NULL) {
			std::cout << "Game window not found..." << std::endl;
			std::cout << "Shutting down..." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(3));
			//close everything
			globals::runhax = false;
			ExitProcess(0);	
		}
		std::this_thread::sleep_for(std::chrono::seconds(30));
	}
}


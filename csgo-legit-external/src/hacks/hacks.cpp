#include "hacks.h"
#include <thread>
#include <iostream>
#include "../utils/memory.h"
#include "../utils/vector.h"
#include "../utils/driver.h"
#include "../../globals.h"
#include "../gui/gui_esp.h"
#include "../gui/gui_menu.h"

constexpr Vector3 CalculateAngle(
	const Vector3& localPosition,
	const Vector3& enemyPosition,
	const Vector3& viewAngles) noexcept
{
	return ((enemyPosition - localPosition).ToAngle() - viewAngles);
}


void hax::readGlobals() noexcept
{
	while (globals::runhax) {
		

		globals::localPlayer = Driver::rpm<std::uint32_t>(globals::client + offset::dwLocalPlayer);
		globals::localTeam = Driver::rpm<std::int32_t>(globals::localPlayer + offset::m_iTeamNum);
		globals::localPlayerFlags = Driver::rpm<std::uint32_t>(globals::localPlayer + offset::m_fFlags);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	
}

void hax::aim() noexcept
{
	while (globals::runhax)
	{
		if (globals::isAim) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			// aimbot key
			if (!GetAsyncKeyState(globals::aimKey))
				continue;



			// eye position = origin + viewOffset
			const auto localEyePosition = Driver::rpm<Vector3>(globals::localPlayer + offset::m_vecOrigin) +
				Driver::rpm<Vector3>(globals::localPlayer + offset::m_vecViewOffset);

			const auto clientState = Driver::rpm<std::uint32_t>(globals::engine + offset::dwClientState);

			const auto localPlayerId =
				Driver::rpm<std::int32_t>(clientState + offset::dwClientState_GetLocalPlayer);

			//bunny(localPlayer, localTeam, localPlayerFlags, memory, client);

			const auto viewAngles = Driver::rpm<Vector3>(clientState + offset::dwClientState_ViewAngles);
			const auto aimPunch = Driver::rpm<Vector3>(globals::localPlayer + offset::m_aimPunchAngle) * 2;

			// aimbot fov
			auto bestFov = 50.f;
			auto bestAngle = Vector3{ };

			for (auto i = 1; i <= 32; ++i)
			{
				const auto player = Driver::rpm<std::uint32_t>(globals::client + offset::dwEntityList + i * 0x10);

				if (Driver::rpm<std::int32_t>(player + offset::m_iTeamNum) == globals::localTeam)
					continue;

				if (Driver::rpm<bool>(player + offset::m_bDormant))
					continue;

				if (Driver::rpm<std::int32_t>(player + offset::m_lifeState))
					continue;

				if (Driver::rpm<std::int32_t>(player + offset::m_bSpottedByMask) & (1 << localPlayerId))
				{
					const auto boneMatrix = Driver::rpm<std::uint32_t>(player + offset::m_dwBoneMatrix);

					// pos of player head in 3d space
					// 8 is the head bone index :)
					const auto playerHeadPosition = Vector3{
						Driver::rpm<float>(boneMatrix + 0x30 * 8 + 0x0C),
						Driver::rpm<float>(boneMatrix + 0x30 * 8 + 0x1C),
						Driver::rpm<float>(boneMatrix + 0x30 * 8 + 0x2C)
					};

					const auto angle = CalculateAngle(
						localEyePosition,
						playerHeadPosition,
						viewAngles + aimPunch
					);

					const auto fov = hypot(angle.x, angle.y);



					if (fov < bestFov)
					{
						bestFov = fov;
						bestAngle = angle;
					}
				}
			}
			// if we have a best angle, do aimbot
			if (!bestAngle.IsZero()) {
				Driver::wpm<Vector3>(clientState + offset::dwClientState_ViewAngles, viewAngles + bestAngle); // smoothing
			}
		}


	}
}



void hax::bunny() noexcept
{
	while (globals::runhax)
	{
		if (globals::isBunny) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			// bunny key
			if (!GetAsyncKeyState(VK_SPACE))
				continue;
			
			if (globals::localPlayerFlags & (1 << 0))
			{
				Driver::wpm<std::int32_t>(globals::client + offset::dwForceJump, 5);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				Driver::wpm<std::int32_t>(globals::client + offset::dwForceJump, 4);
			}
		}
	}
}

void hax::radar() noexcept {

	while (globals::runhax)
	{
		if (globals::isRadar) {

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			for (auto i = 1; i <= 32; ++i)
			{
				const auto player = Driver::rpm<std::uintptr_t>(globals::client + offset::dwEntityList + i * 0x10);

				if (Driver::rpm<std::int32_t>(player + offset::m_iTeamNum) == globals::localTeam)
					continue;

				if (Driver::rpm<bool>(player + offset::m_bDormant))
					continue;

				if (Driver::rpm<std::int32_t>(player + offset::m_lifeState))
					continue;

				Driver::wpm<bool>(player + offset::m_bSpotted, true);
			}
		}
	}
}

void hax::legitAim() noexcept
{
	while (globals::runhax)
	{
		if (globals::isAimAssit) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			// aimbot key
			if (!GetAsyncKeyState(globals::aimAssitKey))
				continue;

			// get local player
			const auto localPlayer = Driver::rpm<std::uint32_t>(globals::client + offset::dwLocalPlayer);
			const auto localTeam = Driver::rpm<std::int32_t>(localPlayer + offset::m_iTeamNum);
			const auto localPlayerFlags = Driver::rpm<std::uint32_t>(localPlayer + offset::m_fFlags);


			// eye position = origin + viewOffset
			const auto localEyePosition = Driver::rpm<Vector3>(localPlayer + offset::m_vecOrigin) +
				Driver::rpm<Vector3>(localPlayer + offset::m_vecViewOffset);

			const auto clientState = Driver::rpm<std::uint32_t>(globals::engine + offset::dwClientState);

			const auto localPlayerId =
				Driver::rpm<std::int32_t>(clientState + offset::dwClientState_GetLocalPlayer);

			const auto viewAngles = Driver::rpm<Vector3>(clientState + offset::dwClientState_ViewAngles);
			const auto aimPunch = Driver::rpm<Vector3>(localPlayer + offset::m_aimPunchAngle) * 2;

			// aimbot fov
			auto bestFov = 2.f;
			auto bestAngle = Vector3{};

			for (auto i = 1; i <= 32; ++i)
			{
				const auto player = Driver::rpm<std::uint32_t>(globals::client + offset::dwEntityList + i * 0x10);

				if (Driver::rpm<std::int32_t>(player + offset::m_iTeamNum) == localTeam)
					continue;

				if (Driver::rpm<bool>(player + offset::m_bDormant))
					continue;

				if (Driver::rpm<std::int32_t>(player + offset::m_lifeState))
					continue;
				//isvisble check
				if (Driver::rpm<std::int32_t>(player + offset::m_bSpottedByMask) & (1 << localPlayerId))
				{
					const auto boneMatrix = Driver::rpm<std::uint32_t>(player + offset::m_dwBoneMatrix);

					// pos of player head in 3d space
					// 8 is the head bone index :)
					const auto playerHeadPosition = Vector3{
						Driver::rpm<float>(boneMatrix + 0x30 * 8 + 0x0C),
						Driver::rpm<float>(boneMatrix + 0x30 * 8 + 0x1C),
						Driver::rpm<float>(boneMatrix + 0x30 * 8 + 0x2C)
					};

					const auto angle = CalculateAngle(
						localEyePosition,
						playerHeadPosition,
						viewAngles + aimPunch
					);

					const auto fov = std::hypot(angle.x, angle.y);

					if (fov < bestFov)
					{
						bestFov = fov;
						bestAngle = angle;
					}
				}
			}

			// if we have a best angle, do aimbot
			if (!bestAngle.IsZero())
			{
				auto targetAngle = viewAngles + bestAngle;

				// calculate delta angle
				const auto deltaAngle = targetAngle - viewAngles;

				// calculate new angle
				const auto newAngle = viewAngles + deltaAngle * globals::legitAimSmooth;

				// write new angle
				Driver::wpm<Vector3>(clientState + offset::dwClientState_ViewAngles, newAngle);
			}
		}
	}
}

void hax::noRecoil() noexcept
{
	auto oldPunch = Vector3{};
	while (globals::runhax)
	{
		if (globals::isNoRecoil) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			const auto localPlayer = Driver::rpm<std::uint32_t>(globals::client + offset::dwLocalPlayer);
			const auto localTeam = Driver::rpm<std::int32_t>(localPlayer + offset::m_iTeamNum);
			const auto localPlayerFlags = Driver::rpm<std::uint32_t>(localPlayer + offset::m_fFlags);
			const auto clientState = Driver::rpm<std::uint32_t>(globals::engine + offset::dwClientState);
			const auto localPlayerId =
				Driver::rpm<std::int32_t>(clientState + offset::dwClientState_GetLocalPlayer);
			const auto shotsFired = Driver::rpm<std::int32_t>(localPlayer + offset::m_iShotsFired);
			if (shotsFired > 1) {
				const auto aimPunch = Driver::rpm<Vector3>(localPlayer + offset::m_aimPunchAngle);
				const auto viewAngles = Driver::rpm<Vector3>(clientState + offset::dwClientState_ViewAngles);
				auto newAngle = viewAngles + oldPunch - aimPunch * 2;

				//angle limit check
				if (newAngle.x > 89.0f)
					newAngle.x = 89.0f;

				if (newAngle.x < -89.0f)
					newAngle.x = -89.0f;

				if (newAngle.y > 180.0f)
					newAngle.y = 180.0f;

				if (newAngle.y < -180.0f)
					newAngle.y = -180.0f;


				Driver::wpm<Vector3>(clientState + offset::dwClientState_ViewAngles, newAngle);
				oldPunch = aimPunch * 2;
			}
			else {
				oldPunch = 0;
			}


		}
	}
}

void hax::trigger() noexcept{
	while (globals::runhax) {
		if (globals::isTrigger) {
			if (!GetAsyncKeyState(globals::triggerKey))
				continue;

			ULONG my_player = Driver::rpm<ULONG>(globals::client + offset::dwLocalPlayer);
			if (my_player <= 0) { continue; }

			ULONG my_team = Driver::rpm<ULONG>(my_player + offset::m_iTeamNum);
			ULONG my_cross = Driver::rpm<ULONG>(my_player + offset::m_iCrosshairId);

			if (my_cross >= 1 && my_cross <= 64)
			{
				ULONG entity = Driver::rpm<ULONG>(globals::client + offset::dwEntityList + ((my_cross - 1) * 0x10));
				if (entity <= 0) { continue; }

				ULONG entity_health = Driver::rpm<ULONG>(entity + offset::m_iHealth);
				ULONG entity_team = Driver::rpm<ULONG>(entity + offset::m_iTeamNum);

				if (entity_health > 0 && entity_team != my_team && entity_team > 1)
				{
					Sleep(3);
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					Sleep(1);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
			}
		}
		
	}
	

}







void hax::esp() noexcept {
	//gui_esp::initWindow();

	//while (globals::run_render) {
	//	gui_esp::initRender();
	//	
	//}
	////gui_esp::shutdown();


}

void hax::menu() noexcept {
	gui_menu::initWindow();
	//gui_menu::initDevice();

	while (globals::run_render) {
		gui_menu::initRender();

	}
	gui_menu::shutdown();
}
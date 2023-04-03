#include "hacks.h"
#include <thread>
#include <iostream>
#include <WinUser.h>
#include "../utils/memory.h"
#include "../utils/vector.h"
#include "../../driver.h"
#include "../../globals.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <d3d11.h>
#include <dwmapi.h>
#include "espstuff.h"

#pragma comment (lib, "dwmapi.lib")
#pragma comment (lib, "d3d11.lib")

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
		if (globals::isaim) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			// aimbot key
			if (!GetAsyncKeyState(0x58))
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
		if (globals::isbunny) {
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
		if (globals::isradar) {

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
		if (globals::islegitAim) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			// aimbot key
			if (!GetAsyncKeyState(0x01))
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

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);

		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void hax::esp() noexcept {
	HINSTANCE hInstance = GetModuleHandle(NULL);
	
	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"ImGui Example";

	ID3D11Device* device = { nullptr };
	ID3D11DeviceContext* context = { nullptr };
	IDXGISwapChain* swap_chain = { nullptr };
	ID3D11RenderTargetView* render_target_view = { nullptr };
	D3D_FEATURE_LEVEL feature_level = {};

	RegisterClassExW(&wcex);

	const HWND window = CreateWindowExW(
		WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
		wcex.lpszClassName,
		L"ImGui Example",
		WS_POPUP,
		0, 0,
		1920, 1080,
		NULL,
		NULL,
		wcex.hInstance,
		NULL
	);

	SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

	{
		RECT client_area = {};
		GetClientRect(window, &client_area);

		RECT window_area = {};
		GetWindowRect(window, &window_area);

		POINT diff = {};
		ClientToScreen(window, &diff);

		const MARGINS margins{
			window_area.left + (diff.x - window_area.left),
			window_area.top + (diff.y - window_area.top),
			client_area.right,
			client_area.bottom
		};

		DwmExtendFrameIntoClientArea(window, &margins);


	}

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = window;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		levels,
		2,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&feature_level,
		&context
	);

	ID3D11Texture2D* back_buffer = { nullptr };
	swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
	if (back_buffer)
	{
		device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
		back_buffer->Release();
	}
	else {
		std::cout << "Failed to get back buffer" << std::endl;
	}

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, context);

	bool running = true;
	while (running) {

		MSG msg = {};
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				running = false;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();



		DWORD localplayer = Driver::rpm<DWORD>(globals::client + offset::dwLocalPlayer);
		DWORD localTeam = Driver::rpm<DWORD>(localplayer + offset::m_iTeamNum);//fixed

		Vector3 punchAngle = Driver::rpm<Vector3>(localplayer + offset::m_aimPunchAngle);

		const auto clientState = Driver::rpm<std::uint32_t>(globals::engine + offset::dwClientState);

		const auto userInfoTable = Driver::rpm<std::uint32_t>(clientState + offset::dwClientState_PlayerInfo);



		const auto localPlayerId =
			Driver::rpm<std::int32_t>(clientState + offset::dwClientState_GetLocalPlayer);


		punchAngle.x = punchAngle.x * 12; punchAngle.y = punchAngle.y * 12;
		float x = 1920 / 2 - punchAngle.y;
		float y = 1080 / 2 + punchAngle.x;


		/*RECT rect = { x - 2, y - 2, x + 2, y + 2 };
		FrameRect(hdc, &rect, brush, 2);*/


		esp::MAT4X4 viewMatrix = Driver::rpm<esp::MAT4X4>(globals::client + offset::dwViewMatrix);

		for (size_t i = 0; i < 32; i++)
		{

			if (globals::isEsp == false)
				continue;
			DWORD currEnt = Driver::rpm<DWORD>(globals::client + offset::dwEntityList + (i * 0x10));
			if (!currEnt)
				continue;

			int entHealth = Driver::rpm<int>(currEnt + offset::m_iHealth);
			if (0 >= entHealth)
				continue;

			DWORD dormant = Driver::rpm<DWORD>(currEnt + offset::m_bDormant);
			if (dormant)
				continue;

			DWORD teamNum = Driver::rpm<DWORD>(currEnt + offset::m_iTeamNum);
			if (teamNum == localTeam)
				continue;

			uintptr_t x = Driver::rpm< std::uintptr_t >(Driver::rpm< uintptr_t >(userInfoTable + 0x40) + 0xC);
			esp::player_info_t p = Driver::rpm<esp::player_info_t >(Driver::rpm< uintptr_t >(x + 0x28 + 0x34 * i));

			Vector3 feetPos = Driver::rpm<Vector3>(currEnt + offset::m_vecOrigin);
			Vector2 feetPosScreen = esp::WorldToScreen(feetPos, viewMatrix);

			DWORD bonePtr = Driver::rpm<DWORD>(currEnt + offset::m_dwBoneMatrix);
			esp::MAT3X4 boneMatrix = Driver::rpm<esp::MAT3X4>(bonePtr + 0x30 * 8); //head
			Vector3 headPos = { boneMatrix.c[0][3], boneMatrix.c[1][3], boneMatrix.c[2][3] };
			headPos.z += 8.75;
			Vector2 headScreen = WorldToScreen(headPos, viewMatrix);

			int height = headScreen.y - feetPosScreen.y;
			int width = height / 4;

			float Entity_x = feetPosScreen.x - width;
			float Entity_y = feetPosScreen.y;
			float Entity_w = height / 2;


			RECT boxEsp = { Entity_x + Entity_w, Entity_y + height, Entity_x, Entity_y };
			//draw health
			int health = entHealth;
			int healthBar = (int)(height * (health / 100.f));
			int healthBarY = Entity_y + height - healthBar;
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(Entity_x + 5, Entity_y + height), ImVec2(Entity_x, healthBarY), ImColor(0, 255, 0));
			//draw health as text
			std::string healthText = std::to_string(health);
			healthText += "%";
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(Entity_x - 5, healthBarY), ImColor(255, 255, 255), healthText.c_str());
			//draw name
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(Entity_x + Entity_w, Entity_y + 10), ImColor(255, 255, 255), p.name);


			if (Driver::rpm<std::int32_t>(currEnt + offset::m_bSpottedByMask) & (1 << localPlayerId)) {

				ImGui::GetBackgroundDrawList()->AddRect(ImVec2(Entity_x + Entity_w, Entity_y + height), ImVec2(Entity_x, Entity_y), ImColor(0, 0, 255));

			}
			else {
				ImGui::GetBackgroundDrawList()->AddRect(ImVec2(Entity_x + Entity_w, Entity_y + height), ImVec2(Entity_x, Entity_y), ImColor(255, 0, 0));

			}


		}




		ImGui::End();



		ImGui::Render();

		constexpr float color[4]{ 0.f, 0.f, 0.f, 0.f };
		context->OMSetRenderTargets(1, &render_target_view, nullptr);
		context->ClearRenderTargetView(render_target_view, color);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		swap_chain->Present(1, 0);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (render_target_view)
		render_target_view->Release();
	if (swap_chain)
		swap_chain->Release();
	if (context)
		context->Release();
	if (device)
		device->Release();

	DestroyWindow(window);
	UnregisterClassW(wcex.lpszClassName, wcex.hInstance);

}
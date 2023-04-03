#include "gui_esp.h"
#include "../../globals.h"
#include "../utils/driver.h"
#include "../utils/vector.h"
#include "../hacks/espstuff.h"

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

void gui_esp::initWindow() {
	HINSTANCE hInstance = GetModuleHandle(NULL);
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"ImGui Example";

	RegisterClassExW(&wcex);

	window = CreateWindowExW(
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
}

void gui_esp::initDevice() {
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
}

void gui_esp::initRender() {
	MSG msg = {};
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			globals::run_render = false;

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

void gui_esp::shutdown() {
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
#include "gui_menu.h"
#include "../../globals.h"
#include "../utils/driver.h"
#include "gui_esp.h"
#include "main_gui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


//
//void gui_menu::initWindow() {
//	HINSTANCE hInstance = GetModuleHandle(NULL);
//	wcexMenu.cbSize = sizeof(WNDCLASSEX);
//	wcexMenu.style = CS_CLASSDC;
//	wcexMenu.lpfnWndProc = WndProcMenu;
//	wcexMenu.hInstance = hInstance;
//	wcexMenu.lpszClassName = L"ImGui Example";
//
//	RegisterClassExW(&wcexMenu);
//
//	windowMenu = CreateWindowExW(
//		0,
//		wcexMenu.lpszClassName,
//		wcexMenu.lpszClassName,
//		WS_POPUP,
//		100,
//		100,
//		200,
//		200,
//		0,
//		0,
//		wcexMenu.hInstance,
//		0
//	);
//
//	ShowWindow(windowMenu, SW_SHOWDEFAULT);
//	UpdateWindow(windowMenu);
//	
//}
//
//void gui_menu::initDevice() {
//	DXGI_SWAP_CHAIN_DESC sd = {};
//	sd.BufferDesc.RefreshRate.Numerator = 60;
//	sd.BufferDesc.RefreshRate.Denominator = 1;
//	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	sd.SampleDesc.Count = 1;
//	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	sd.BufferCount = 2;
//	sd.OutputWindow = windowMenu;
//	sd.Windowed = TRUE;
//	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//
//	constexpr D3D_FEATURE_LEVEL levels[2]{
//		D3D_FEATURE_LEVEL_11_0,
//		D3D_FEATURE_LEVEL_10_0
//	};
//
//	D3D11CreateDeviceAndSwapChain(
//		nullptr,
//		D3D_DRIVER_TYPE_HARDWARE,
//		nullptr,
//		0,
//		levels,
//		2,
//		D3D11_SDK_VERSION,
//		&sd,
//		&swapChainMenu,
//		&deviceMenu,
//		&featureLevelMenu,
//		&contextMenu
//	);
//
//	ID3D11Texture2D* back_buffer = { nullptr };
//	swapChainMenu->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
//	if (back_buffer)
//	{
//		deviceMenu->CreateRenderTargetView(back_buffer, nullptr, &renderTargetMenu);
//		back_buffer->Release();
//	}
//	else {
//		std::cout << "Failed to get back buffer" << std::endl;
//	}
//
//	
//
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ::ImGui::GetIO();
//
//	io.IniFilename = NULL;
//
//	ImGuiStyle* style = &ImGui::GetStyle();
//
//	style->WindowTitleAlign = { 0.5f, 0.5f };
//	style->WindowPadding = { 15, 15 };
//	style->ChildRounding = 2.f;
//	style->WindowRounding = 0.f;
//	style->ScrollbarRounding = 1.f;
//	style->FrameRounding = 2.f;
//	style->ItemSpacing = { 8, 8 };
//	style->ScrollbarSize = 3.f;
//
//	ImVec4* colors = ImGui::GetStyle().Colors;
//	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
//	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
//	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.96f);
//	colors[ImGuiCol_ChildBg] = ImVec4(0.11f, 0.11f, 0.14f, 1.00f);
//	colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
//	colors[ImGuiCol_Border] = ImVec4(0.32f, 0.32f, 0.58f, 0.30f);
//	colors[ImGuiCol_BorderShadow] = ImVec4(0.17f, 0.00f, 0.52f, 0.26f);
//	colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.27f, 0.38f, 0.54f);
//	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.29f, 0.37f, 0.62f, 0.54f);
//	colors[ImGuiCol_FrameBgActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
//	colors[ImGuiCol_TitleBg] = ImVec4(0.33f, 0.33f, 0.68f, 1.00f);
//	colors[ImGuiCol_TitleBgActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
//	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
//	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
//	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
//	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
//	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
//	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
//	colors[ImGuiCol_CheckMark] = ImVec4(0.81f, 0.66f, 1.00f, 1.00f);
//	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
//	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
//	colors[ImGuiCol_Button] = ImVec4(0.35f, 0.37f, 0.48f, 0.40f);
//	colors[ImGuiCol_ButtonHovered] = ImVec4(0.33f, 0.35f, 0.49f, 1.00f);
//	colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
//	colors[ImGuiCol_Header] = ImVec4(0.42f, 0.32f, 0.67f, 1.00f);
//	colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.41f, 0.73f, 1.00f);
//	colors[ImGuiCol_HeaderActive] = ImVec4(0.33f, 0.33f, 0.67f, 1.00f);
//	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
//	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
//	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
//	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
//	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
//	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
//	colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.50f, 1.00f, 0.35f);
//	colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.29f, 0.84f, 1.00f);
//	colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.24f, 0.80f, 1.00f);
//	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
//	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
//	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
//	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
//	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
//	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
//	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
//	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
//	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
//	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
//	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
//	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
//	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
//	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
//	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
//	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
//	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
//
//	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\tahoma.ttf", 16.0f);
//
//
//	ImGui_ImplWin32_Init(windowMenu);
//	ImGui_ImplDX11_Init(deviceMenu, contextMenu);
//}
//
//void gui_menu::initRender() {
//	MSG msg = {};
//	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//	{
//		if (msg.message == WM_QUIT)
//			globals::run_render = false;
//
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//
//	ImGui_ImplDX11_NewFrame();
//	ImGui_ImplWin32_NewFrame();
//	ImGui::NewFrame();
//	ImGui::SetNextWindowPos({ 0, 0 });
//	ImGui::SetNextWindowSize({ 200, 200 });
//	ImGui::Begin(
//		"Externalia",
//		&globals::run_render,
//		ImGuiWindowFlags_NoResize |
//		ImGuiWindowFlags_NoSavedSettings |
//		ImGuiWindowFlags_NoCollapse |
//		ImGuiWindowFlags_NoMove
//	);
//
//	static auto current_tab = 0;
//
//	if (ImGui::BeginChild(
//		1,
//		{ ImGui::GetContentRegionAvail().x * 0.25f, ImGui::GetContentRegionAvail().y },
//		true)) {
//		constexpr auto button_height = 48;
//		if (ImGui::Button("Aimbot", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 0; }
//		if (ImGui::Button("Visuals", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 1; }
//		if (ImGui::Button("Other", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 2; }
//		if (ImGui::Button("Skins", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 3; }
//
//		ImGui::EndChild();
//	}
//
//	ImGui::SameLine();
//
//	if (ImGui::BeginChild(
//		2,
//		ImGui::GetContentRegionAvail(),
//		true)) {
//
//		switch (current_tab) {
//		case 0:
//			break;
//
//		case 1:
//			ImGui::Checkbox("ESP", &globals::isEsp);/*
//			ImGui::ColorEdit4("color##team_glow", v::team_glow.second.data());*/
//
//			ImGui::Spacing();
//
//			/*ImGui::Checkbox("enemy glow", &v::enemy_glow.first);
//			ImGui::ColorEdit4("color##enemy_glow", v::enemy_glow.second.data());*/
//			break;
//
//		case 2:
//			ImGui::Checkbox("radar", &globals::isRadar);
//			break;
//
//		case 3:
//			break;
//		}
//
//		ImGui::EndChild();
//	}
//
//
//
//
//
//
//
//	ImGui::End();
//
//
//
//	ImGui::Render();
//
//	constexpr float color[4]{ 0.f, 0.f, 0.f, 0.f };
//	contextMenu->OMSetRenderTargets(1, &renderTargetMenu, nullptr);
//	contextMenu->ClearRenderTargetView(renderTargetMenu, color);
//	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//
//	swapChainMenu->Present(1, 0);
//}
//
//void gui_menu::shutdown() {
//	ImGui_ImplDX11_Shutdown();
//	ImGui_ImplWin32_Shutdown();
//	ImGui::DestroyContext();
//
//	if (renderTargetMenu)
//		renderTargetMenu->Release();
//	if (swapChainMenu)
//		swapChainMenu->Release();
//	if (contextMenu)
//		contextMenu->Release();
//	if (deviceMenu)
//		deviceMenu->Release();
//
//	DestroyWindow(windowMenu);
//	UnregisterClassW(wcexMenu.lpszClassName, wcexMenu.hInstance);
//}

//D3D
ID3D11Device* g_pd3dDevice = NULL;
ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
IDXGISwapChain* g_pSwapChain = NULL;
ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

//Imgui
ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

bool vsync = 1;
WNDCLASS wcMenu = {};


void imgui_init_style()
{

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGuiStyle* style = &ImGui::GetStyle();
	auto font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\calibrib.ttf", 15.0f);
	io.FontGlobalScale = 1.1;
	//ImGui::PushFont(font);


	style->Alpha = 1.f;
	style->WindowRounding = 0.7f;
	style->FrameRounding = 4.0f;

	style->WindowBorderSize = 1.8f;
	style->ChildBorderSize = 1.8f;

	style->ItemInnerSpacing = ImVec2(4, 4);
	style->ItemSpacing = ImVec2(8, 4);


	style->ScrollbarSize = 10.f;
	style->ScrollbarRounding = 5.f;

	style->WindowMinSize = ImVec2(425, 360);


	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImColor(100, 100, 100, 255);
	colors[ImGuiCol_ChildBg] = ImColor(100, 100, 100, 255);
	colors[ImGuiCol_PopupBg] = ImColor(100, 100, 100, 255);

	colors[ImGuiCol_Border] = ImColor(0, 0, 0, 255);
	colors[ImGuiCol_FrameBg] = ImColor(100, 149, 237, 255);
	//colors[ImGuiCol_FrameBgActive] = ImColor(139, 0, 139, 255);
	//colors[ImGuiCol_TitleBg] = ImColor(139, 0, 139, 255);
	colors[ImGuiCol_CheckMark] = ImColor(100, 100, 100, 255);


	//colors[ImGuiCol_FrameBgHovered] = ImColor(255, 0, 0, 255);

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls



	io.IniFilename = nullptr;
	io.LogFilename = nullptr;


	return;
}


void gui_menu::initWindow()
{
	m_gui::gameHWND = FindWindow(0, _T("Counter-Strike: Global Offensive - Direct3D 9"));
	if (m_gui::gameHWND == NULL)
	{
		MessageBox(0, "[GuiEngine MENU] Game not found", "ERROR", MB_OK | MB_ICONERROR);
		
	}

	HINSTANCE hInstance = GetModuleHandle(NULL);

	//wcMenu = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, GuiEngine::WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)),NULL, (LPCWSTR)randomStrGen(10), NULL };
	wcMenu.cbClsExtra = 0;
	wcMenu.cbWndExtra = 0;
	wcMenu.hCursor = LoadCursor(0, IDC_ARROW);
	wcMenu.hIcon = LoadIcon(0, IDI_WINLOGO);
	wcMenu.lpszMenuName = 0;
	wcMenu.style = 0;
	wcMenu.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wcMenu.lpfnWndProc = m_gui::WndProc;
	wcMenu.hInstance = hInstance;
	wcMenu.lpszClassName = "M3NU Window";

	RegisterClass(&wcMenu);

	
	//cheatMenuHWND = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW, wcMenu.lpszClassName, wcMenu.lpszMenuName, WS_POPUP, 1, 1, rc.right - rc.left, rc.bottom - rc.top, 0, 0, 0, 0);  // |  WS_EX_CLIENTEDGE| WS_EX_TRANSPARENT 
	windowMenu = CreateWindowEx
	(
		WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		"M3NU Window",
		"Hack",
		WS_POPUP,
		100,
		100,
		1000,
		600,
		NULL,
		NULL,
		wcMenu.hInstance,
		NULL
	);

	SetLayeredWindowAttributes(windowMenu, RGB(0, 0, 0), 0, /*LWA_ALPHA | */LWA_COLORKEY);

	MARGINS Margin = { -1 };
	DwmExtendFrameIntoClientArea(windowMenu, &Margin);

	/*ShowWindow(windowMenu, SW_SHOWDEFAULT);
	UpdateWindow(windowMenu);*/

	if (!gui_menu::create_device_D3D(windowMenu))
	{
		MessageBox(0, "[ GuiEngine Menu ] create_device_D3D error", "ERROR", MB_OK | MB_ICONERROR);
		gui_menu::cleanup_device_D3D();
		UnregisterClass(wcMenu.lpszClassName, wcMenu.hInstance);

	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//ImGui::StyleColorsClassic();
	ImGui::StyleColorsDark();

	imgui_init_style();

	ImGui_ImplWin32_Init(windowMenu);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

}

bool gui_menu::create_device_D3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT hrs;
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
	{
		return false;
	}

	create_render_target();
	return true;
}
void gui_menu::cleanup_device_D3D()
{
	gui_menu::cleanup_render_target();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void gui_menu::create_render_target()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}
void gui_menu::cleanup_render_target()
{
	if (g_mainRenderTargetView)
	{
		g_mainRenderTargetView->Release();
		g_mainRenderTargetView = NULL;
	}

}

void begin_draw_menu()
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void end_draw_menu()
{
	ImGui::End();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	g_pSwapChain->Present((UINT)vsync, 0); // Present with vsync
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
}
void clear_window()
{
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
	g_pSwapChain->Present((UINT)vsync, 0); // Present with vsync
}

void gui_menu::shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	cleanup_device_D3D();
	cleanup_render_target();
	DestroyWindow(windowMenu);
	UnregisterClass(wcMenu.lpszClassName, wcMenu.hInstance);

}

void gui_menu::initRender() {
	begin_draw_menu();
	//
	static auto current_tab = 0;

	MSG message;
	while (PeekMessage(
		&message,
		0,
		0,
		0,
		PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT) {
			globals::run_render = false;
			return;
		}
	}
	if (GetAsyncKeyState(VK_INSERT) & 0x8000)
	{
		globals::isMenu = !globals::isMenu;
		if (globals::isMenu)
		{
			ShowWindow(windowMenu, SW_SHOWDEFAULT);
			SetForegroundWindow(windowMenu);
			SetActiveWindow(windowMenu);
		}
		else
		{
			ShowWindow(windowMenu, SW_HIDE);
			SetForegroundWindow(m_gui::gameHWND);
			SetActiveWindow(m_gui::gameHWND);
		}
		Sleep(400);
	}

	if (globals::isMenu)
	{
		ShowWindow(windowMenu, SW_SHOWDEFAULT);
	}
	else
	{
		ShowWindow(windowMenu, SW_HIDE);
	}

	if (globals::isMenu && (GetForegroundWindow() == m_gui::gameHWND) || (GetForegroundWindow() == windowMenu)) {
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize({ 500, 300 });
		ImGui::Begin(
			"eren.exe",
			&globals::run_render,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar
		);


		if (ImGui::BeginChild(
			1,
			{ ImGui::GetContentRegionAvail().x * 0.25f, ImGui::GetContentRegionAvail().y },
			true)) {
			constexpr auto button_height = 48;
			if (ImGui::Button("Aimbot", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 0; }
			if (ImGui::Button("Visuals", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 1; }
			if (ImGui::Button("Other", { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 2; }

			ImGui::EndChild();
		}

		ImGui::SameLine();

		if (ImGui::BeginChild(
			2,
			ImGui::GetContentRegionAvail(),
			true)) {

			switch (current_tab) {
			case 0:
				break;

			case 1:
				ImGui::Checkbox("ESP", &globals::isEsp);

				ImGui::Spacing();


				break;

			case 2:
				ImGui::Checkbox("radar", &globals::isRadar);
				break;

			}

			ImGui::EndChild();
		}



		end_draw_menu();
	}

	
}
#include "../../globals.h"
#include "gui_menu.h"
#include "../utils/driver.h"
#include "gui_esp.h"
#include "main_gui.h"
#include "../utils/obfuscate.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

	ImGuiStyle& Style = ImGui::GetStyle();
	auto Color = Style.Colors;

	Style.WindowMinSize = ImVec2(500, 300);
	Style.WindowBorderSize = 0;

	Style.ChildRounding = 0;
	Style.FrameRounding = 0;
	Style.ScrollbarRounding = 0;
	Style.GrabRounding = 0;
	Style.PopupRounding = 0;
	Style.WindowRounding = 3;


	Color[ImGuiCol_WindowBg] = ImColor(18, 18, 18, 255);

	Color[ImGuiCol_FrameBg] = ImColor(31, 31, 31, 255);
	Color[ImGuiCol_FrameBgActive] = ImColor(41, 41, 41, 255);
	Color[ImGuiCol_FrameBgHovered] = ImColor(41, 41, 41, 255);

	Color[ImGuiCol_Button] = ImColor(168, 50, 133);
	Color[ImGuiCol_ButtonActive] = ImColor(168, 50, 133);
	Color[ImGuiCol_ButtonHovered] = ImColor(168, 50, 133);

	Color[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
	Color[ImGuiCol_Separator] = ImColor(36, 36, 36, 255);

	Color[ImGuiCol_ResizeGrip] = ImColor(30, 30, 30, 255);
	Color[ImGuiCol_ResizeGripActive] = ImColor(30, 30, 30, 255);
	Color[ImGuiCol_ResizeGripHovered] = ImColor(30, 30, 30, 255);

	Color[ImGuiCol_ChildBg] = ImColor(26, 26, 26, 255);

	Color[ImGuiCol_ScrollbarBg] = ImColor(24, 24, 24, 255);
	Color[ImGuiCol_ScrollbarGrab] = ImColor(24, 24, 24, 255);
	Color[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);
	Color[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);

	Color[ImGuiCol_Header] = ImColor(39, 39, 39, 255);
	Color[ImGuiCol_HeaderActive] = ImColor(39, 39, 39, 255);
	Color[ImGuiCol_HeaderHovered] = ImColor(39, 39, 39, 255);
	Color[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
}

void gui_menu::initWindow()
{


	m_gui::gameHWND = FindWindow(0, _T(AY_OBFUSCATE("Counter-Strike: Global Offensive - Direct3D 9")));
	if (m_gui::gameHWND == NULL)
	{
		MessageBox(0, AY_OBFUSCATE("[GuiEngine MENU] Game not found"), AY_OBFUSCATE("ERROR"), MB_OK | MB_ICONERROR);

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
	wcMenu.lpszClassName = AY_OBFUSCATE("M3NU Window");

	RegisterClass(&wcMenu);


	//cheatMenuHWND = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW, wcMenu.lpszClassName, wcMenu.lpszMenuName, WS_POPUP, 1, 1, rc.right - rc.left, rc.bottom - rc.top, 0, 0, 0, 0);  // |  WS_EX_CLIENTEDGE| WS_EX_TRANSPARENT 
	windowMenu = CreateWindowEx
	(
		WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		AY_OBFUSCATE("M3NU Window"),
		AY_OBFUSCATE("Hack"),
		WS_POPUP,
		0,
		0,
		500,
		300,
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
		MessageBox(0, AY_OBFUSCATE("[ GuiEngine Menu ] create_device_D3D error"), AY_OBFUSCATE("ERROR"), MB_OK | MB_ICONERROR);
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
		//ImGui::SetNextWindowSize({ 750, 450 });
		ImGui::Begin(
			AY_OBFUSCATE("eren.exe"),
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
			if (ImGui::Button(AY_OBFUSCATE("Aimbot"), { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 0; }
			if (ImGui::Button(AY_OBFUSCATE("Visuals"), { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 1; }
			if (ImGui::Button(AY_OBFUSCATE("Other"), { ImGui::GetContentRegionAvail().x, button_height })) { current_tab = 2; }

			ImGui::EndChild();
		}

		ImGui::SameLine();

		if (ImGui::BeginChild(
			2,
			ImGui::GetContentRegionAvail(),
			true)) {

			switch (current_tab) {
			case 0:
				//create new column
				ImGui::Columns(2, nullptr, false);
				//create slider for fov
				ImGui::SliderFloat(AY_OBFUSCATE("FOV"), &globals::aimFov, 0, 100);
				ImGui::Checkbox(AY_OBFUSCATE("AIM"), &globals::isAimAssit);
				ImGui::Spacing();
				ImGui::SliderFloat(AY_OBFUSCATE("SMOOTH"), &globals::legitAimSmooth, 0.1f, 1.f);
				ImGui::NextColumn();
				aimassist_hotkey.render();
				ImGui::NextColumn();
				ImGui::Checkbox(AY_OBFUSCATE("TRIGGER"), &globals::isTrigger);
				ImGui::NextColumn();
				trigger_hotkey.render();
				ImGui::NextColumn();
				ImGui::Checkbox(AY_OBFUSCATE("NORECOIL"), &globals::isNoRecoil);
				break;

			case 1:
				ImGui::Checkbox(AY_OBFUSCATE("ESP"), &globals::isEsp);
				ImGui::Spacing();


				break;

			case 2:
				ImGui::Checkbox(AY_OBFUSCATE("RADAR"), &globals::isRadar);
				ImGui::Spacing();
				ImGui::Checkbox(AY_OBFUSCATE("BUNNY"), &globals::isBunny);
				ImGui::Spacing();
				ImGui::Checkbox(AY_OBFUSCATE("N0 FLASH"), &globals::isFlash);
				ImGui::Spacing();
				ImGui::Checkbox(AY_OBFUSCATE("AUTO ACCEPT"), &globals::isAutoAccept);
				break;

			}

			ImGui::EndChild();
		}



		end_draw_menu();
	}


}

void gui_menu::startMenu()
{
	initWindow();
	//gui_menu::initDevice();

	while (globals::run_render) {
		initRender();

	}
	shutdown();
}
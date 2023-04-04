#include "gui_esp.h"
#include "../../globals.h"
#include "../utils/driver.h"
#include "../utils/vector.h"
#include "../hacks/espstuff.h"
#include "gui_menu.h"
#include "main_gui.h"

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
//		return true;
//
//	switch (message)
//	{
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	case WM_KEYDOWN:
//		if (wParam == VK_ESCAPE)
//			DestroyWindow(hWnd);
//
//		return 0;
//	}
//	return DefWindowProc(hWnd, message, wParam, lParam);
//}
//
//void gui_esp::initWindow() {
//	HINSTANCE hInstance = GetModuleHandle(NULL);
//	wcexEsp.cbSize = sizeof(WNDCLASSEX);
//	wcexEsp.style = CS_HREDRAW | CS_VREDRAW;
//	wcexEsp.lpfnWndProc = WndProc;
//	wcexEsp.hInstance = hInstance;
//	wcexEsp.lpszClassName = L"ImGui Example";
//
//	RegisterClassExW(&wcexEsp);
//
//	windowEsp = CreateWindowExW(
//		WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
//		wcexEsp.lpszClassName,
//		L"ImGui Example",
//		WS_POPUP,
//		0, 0,
//		1920, 1080,
//		NULL,
//		NULL,
//		wcexEsp.hInstance,
//		NULL
//	);
//
//	SetLayeredWindowAttributes(windowEsp, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);
//
//	{
//		RECT client_area = {};
//		GetClientRect(windowEsp, &client_area);
//
//		RECT window_area = {};
//		GetWindowRect(windowEsp, &window_area);
//
//		POINT diff = {};
//		ClientToScreen(windowEsp, &diff);
//
//		const MARGINS margins{
//			window_area.left + (diff.x - window_area.left),
//			window_area.top + (diff.y - window_area.top),
//			client_area.right,
//			client_area.bottom
//		};
//
//		DwmExtendFrameIntoClientArea(windowEsp, &margins);
//
//
//	}
//}
//
//void gui_esp::initDevice() {
//	DXGI_SWAP_CHAIN_DESC sd = {};
//	sd.BufferDesc.RefreshRate.Numerator = 60;
//	sd.BufferDesc.RefreshRate.Denominator = 1;
//	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	sd.SampleDesc.Count = 1;
//	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	sd.BufferCount = 2;
//	sd.OutputWindow = windowEsp;
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
//		&swapChainEsp,
//		&deviceEsp,
//		&featureLevelEsp,
//		&contextEsp
//	);
//
//	ID3D11Texture2D* back_buffer = { nullptr };
//	swapChainEsp->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
//	if (back_buffer)
//	{
//		deviceEsp->CreateRenderTargetView(back_buffer, nullptr, &renderTargetEsp);
//		back_buffer->Release();
//	}
//	else {
//		std::cout << "Failed to get back buffer" << std::endl;
//	}
//
//	ShowWindow(windowEsp, SW_SHOWDEFAULT);
//	UpdateWindow(windowEsp);
//
//	ImGui::CreateContext();
//	ImGui::StyleColorsDark();
//
//	ImGui_ImplWin32_Init(windowEsp);
//	ImGui_ImplDX11_Init(deviceEsp, contextEsp);
//}
//
//void gui_esp::initRender() {
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
//
//
//
//	DWORD localplayer = Driver::rpm<DWORD>(globals::client + offset::dwLocalPlayer);
//	DWORD localTeam = Driver::rpm<DWORD>(localplayer + offset::m_iTeamNum);//fixed
//
//	Vector3 punchAngle = Driver::rpm<Vector3>(localplayer + offset::m_aimPunchAngle);
//
//	const auto clientState = Driver::rpm<std::uint32_t>(globals::engine + offset::dwClientState);
//
//	const auto userInfoTable = Driver::rpm<std::uint32_t>(clientState + offset::dwClientState_PlayerInfo);
//
//
//
//	const auto localPlayerId =
//		Driver::rpm<std::int32_t>(clientState + offset::dwClientState_GetLocalPlayer);
//
//
//	punchAngle.x = punchAngle.x * 12; punchAngle.y = punchAngle.y * 12;
//	float x = 1920 / 2 - punchAngle.y;
//	float y = 1080 / 2 + punchAngle.x;
//
//
//	/*RECT rect = { x - 2, y - 2, x + 2, y + 2 };
//	FrameRect(hdc, &rect, brush, 2);*/
//
//
//	esp::MAT4X4 viewMatrix = Driver::rpm<esp::MAT4X4>(globals::client + offset::dwViewMatrix);
//
//	for (size_t i = 0; i < 32; i++)
//	{
//
//		if (globals::isEsp == false)
//			continue;
//		DWORD currEnt = Driver::rpm<DWORD>(globals::client + offset::dwEntityList + (i * 0x10));
//		if (!currEnt)
//			continue;
//
//		int entHealth = Driver::rpm<int>(currEnt + offset::m_iHealth);
//		if (0 >= entHealth)
//			continue;
//
//		DWORD dormant = Driver::rpm<DWORD>(currEnt + offset::m_bDormant);
//		if (dormant)
//			continue;
//
//		DWORD teamNum = Driver::rpm<DWORD>(currEnt + offset::m_iTeamNum);
//		if (teamNum == localTeam)
//			continue;
//
//		uintptr_t x = Driver::rpm< std::uintptr_t >(Driver::rpm< uintptr_t >(userInfoTable + 0x40) + 0xC);
//		esp::player_info_t p = Driver::rpm<esp::player_info_t >(Driver::rpm< uintptr_t >(x + 0x28 + 0x34 * i));
//
//		Vector3 feetPos = Driver::rpm<Vector3>(currEnt + offset::m_vecOrigin);
//		Vector2 feetPosScreen = esp::WorldToScreen(feetPos, viewMatrix);
//
//		DWORD bonePtr = Driver::rpm<DWORD>(currEnt + offset::m_dwBoneMatrix);
//		esp::MAT3X4 boneMatrix = Driver::rpm<esp::MAT3X4>(bonePtr + 0x30 * 8); //head
//		Vector3 headPos = { boneMatrix.c[0][3], boneMatrix.c[1][3], boneMatrix.c[2][3] };
//		headPos.z += 8.75;
//		Vector2 headScreen = WorldToScreen(headPos, viewMatrix);
//
//		int height = headScreen.y - feetPosScreen.y;
//		int width = height / 4;
//
//		float Entity_x = feetPosScreen.x - width;
//		float Entity_y = feetPosScreen.y;
//		float Entity_w = height / 2;
//
//
//		RECT boxEsp = { Entity_x + Entity_w, Entity_y + height, Entity_x, Entity_y };
//		//draw health
//		int health = entHealth;
//		int healthBar = (int)(height * (health / 100.f));
//		int healthBarY = Entity_y + height - healthBar;
//		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(Entity_x + 5, Entity_y + height), ImVec2(Entity_x, healthBarY), ImColor(0, 255, 0));
//		//draw health as text
//		std::string healthText = std::to_string(health);
//		healthText += "%";
//		ImGui::GetBackgroundDrawList()->AddText(ImVec2(Entity_x - 5, healthBarY), ImColor(255, 255, 255), healthText.c_str());
//		//draw name
//		ImGui::GetBackgroundDrawList()->AddText(ImVec2(Entity_x + Entity_w, Entity_y + 10), ImColor(255, 255, 255), p.name);
//
//
//		if (Driver::rpm<std::int32_t>(currEnt + offset::m_bSpottedByMask) & (1 << localPlayerId)) {
//
//			ImGui::GetBackgroundDrawList()->AddRect(ImVec2(Entity_x + Entity_w, Entity_y + height), ImVec2(Entity_x, Entity_y), ImColor(0, 0, 255));
//
//		}
//		else {
//			ImGui::GetBackgroundDrawList()->AddRect(ImVec2(Entity_x + Entity_w, Entity_y + height), ImVec2(Entity_x, Entity_y), ImColor(255, 0, 0));
//
//		}
//
//
//	}
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
//	contextEsp->OMSetRenderTargets(1, &renderTargetEsp, nullptr);
//	contextEsp->ClearRenderTargetView(renderTargetEsp, color);
//	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//
//	swapChainEsp->Present(1, 0);
//}
//
//void gui_esp::shutdown() {
//	ImGui_ImplDX11_Shutdown();
//	ImGui_ImplWin32_Shutdown();
//	ImGui::DestroyContext();
//
//	if (renderTargetEsp)
//		renderTargetEsp->Release();
//	if (swapChainEsp)
//		swapChainEsp->Release();
//	if (contextEsp)
//		contextEsp->Release();
//	if (deviceEsp)
//		deviceEsp->Release();
//
//	DestroyWindow(windowEsp);
//	UnregisterClassW(wcexEsp.lpszClassName, wcexEsp.hInstance);
//}

ID2D1Factory* pFactory;
ID2D1HwndRenderTarget* pRenderTarget;
ID2D1SolidColorBrush* ColorBrush;

IDWriteFactory1* pDWriteFactory;
IDWriteTextFormat* TextFormat;
IDWriteTextFormat* BigTextFormat;

D2D1_COLOR_F clear_color_D2D = { 0.0f, 0.0f, 0.0f, 0.0f };
D2D1_COLOR_F color_brush = { 0.0f, 0.0f, 0.0f, 1.0f };

// ESP var
WNDCLASS wcEsp = {};

//Forward declaration of the WndProc function
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool gui_esp::init_window_Esp()
{
	

	HINSTANCE hInstance = GetModuleHandle(NULL);

	wcEsp.cbClsExtra = 0;
	wcEsp.cbWndExtra = 0;
	wcEsp.hCursor = LoadCursor(0, IDC_ARROW);
	wcEsp.hIcon = LoadIcon(0, IDI_WINLOGO);
	wcEsp.lpszMenuName = 0;
	wcEsp.style = 0;
	wcEsp.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wcEsp.lpfnWndProc = m_gui::WndProc;
	wcEsp.hInstance = hInstance;
	wcEsp.lpszClassName = "E5P Window";

	RegisterClass(&wcEsp);
	
	windowEsp = CreateWindowEx
	(
		WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
		"E5P Window",
		"Hack",
		WS_POPUP,
		0,
		0,
		1920,
		1080,
		NULL,
		NULL,
		hInstance,
		NULL
	);


	SetLayeredWindowAttributes(windowEsp, RGB(0, 0, 0), 255, /*ULW_COLORKEY | */LWA_ALPHA);

	MARGINS Margin = { -1 };
	DwmExtendFrameIntoClientArea(windowEsp, &Margin);

	if (!gui_esp::init_render())
	{
		MessageBox(0, "[GuiEngine ESP] init_render error", "ERROR", MB_OK | MB_ICONERROR);
		cleanup_canvas();
		UnregisterClass(wcEsp.lpszClassName, wcEsp.hInstance);
		return 0;
	}

	ShowWindow(windowEsp, SW_SHOWDEFAULT);
	UpdateWindow(windowEsp);

	return 1;
}

bool gui_esp::init_render()
{
	D2D1_FACTORY_OPTIONS CreateOpt = { D2D1_DEBUG_LEVEL_NONE };

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pDWriteFactory), (IUnknown**)&pDWriteFactory);
	pDWriteFactory->CreateTextFormat(L"Arial Unicode MS", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 12.f, L"", &TextFormat);
	pDWriteFactory->CreateTextFormat(L"Arial Unicode MS", NULL, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 20.f, L"", &BigTextFormat);

	if (S_OK != D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))
	{
		MessageBox(0, "D2D1CreateFactory", "ERROR", MB_OK | MB_ICONERROR);
		std::cout << "D2D1CreateFactory error" << std::endl;
		return 0;
	}
	gui_esp::create_canvas();

	return 1;
}

void gui_esp::cleanup_canvas()
{
	if (pRenderTarget)
	{
		pRenderTarget->Release();
		pRenderTarget = nullptr;
	}
}

void gui_esp::create_canvas()
{
	RECT rc;
	GetClientRect(windowEsp, &rc);
	pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)),
		D2D1::HwndRenderTargetProperties(windowEsp, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&pRenderTarget
	);
	if (!pRenderTarget) {
		MessageBox(0, "GuiEngine create_canvas ] !pRenderTarget", "ERROR", MB_OK | MB_ICONERROR);
		std::cout << "cant create canvas" << std::endl;
	}
		

	
	pRenderTarget->SetDpi(96, 96);
	pRenderTarget->CreateSolidColorBrush(color_brush, &ColorBrush);
}

void gui_esp::begin_draw_esp()
{
	if (!pRenderTarget)
	{
		std::cout << "Error pRenderTarget" << std::endl;
		
	}
	pRenderTarget->BeginDraw();
	pRenderTarget->Clear(clear_color_D2D);
}

void gui_esp::clear_window()
{
	gui_esp::begin_draw_esp();
	pRenderTarget->Clear(clear_color_D2D);
	gui_esp::end_draw_esp();
}

void gui_esp::end_draw_esp()
{
	HRESULT state = pRenderTarget->EndDraw();

	//if (state == D2DERR_RECREATE_TARGET)
	//{

	//}
}

void gui_esp::shutdown()
{
	if (pRenderTarget) {
		pRenderTarget->Release();
		pRenderTarget = nullptr;
	}
	if (pFactory) {
		pFactory->Release();
		pFactory = nullptr;
	}
	if (ColorBrush) {
		ColorBrush->Release();
		ColorBrush = nullptr;
	}
	if (pDWriteFactory) {
		pDWriteFactory->Release();
		pDWriteFactory = nullptr;
	}
	if (TextFormat) {
		TextFormat->Release();
		TextFormat = nullptr;
	}
	if (BigTextFormat) {
		BigTextFormat->Release();
		BigTextFormat = nullptr;
	}

	DestroyWindow(windowEsp);
	UnregisterClass(wcEsp.lpszClassName, wcEsp.hInstance);
}

void gui_esp::rect(float x, float y, float h, float w, const D2D1::ColorF& clr, float thick)
{
	ColorBrush->SetColor(clr);
	pRenderTarget->DrawRectangle(D2D1::RectF(x, y, x + h, y + w), ColorBrush, thick);
}

void gui_esp::fill_rect(float x, float y, float w, float h, const D2D1::ColorF& clr)
{
	ColorBrush->SetColor(clr);
	pRenderTarget->FillRectangle(D2D1::RectF(x, y, x + w, y + h), ColorBrush);
}

void gui_esp::Line(const Vector2& start, const Vector2& end, const D2D1::ColorF& clr, float thick)
{
	ColorBrush->SetColor(clr);
	pRenderTarget->DrawLine({ start.x, start.y }, { end.x, end.y }, ColorBrush, thick);
}

void gui_esp::Crosshair(const int& screenWidth, const int& screenHeight, const D2D1::ColorF& clr, const float& thick)
{
	ColorBrush->SetColor(clr);
	float xCenter = screenWidth / 2, yCenter = screenHeight / 2;
	pRenderTarget->DrawLine({ xCenter - 5, yCenter - 5 }, { xCenter + 5, yCenter + 5 }, ColorBrush, thick);
	pRenderTarget->DrawLine({ xCenter + 5, yCenter - 5 }, { xCenter - 5, yCenter + 5 }, ColorBrush, thick);
	ColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::BlueViolet));
	pRenderTarget->DrawRectangle(D2D1::RectF(xCenter, yCenter, xCenter, yCenter), ColorBrush, thick + 1.0f);
}

void gui_esp::Circle(const Vector2& start, const D2D1::ColorF& clr, float rad, float thick)
{
	ColorBrush->SetColor(clr);
	pRenderTarget->DrawEllipse({ {start.x, start.y}, rad, rad }, ColorBrush, thick);
}

void gui_esp::CenterString(const Vector2& pos, const wchar_t* Str, const D2D1::ColorF& Clr, bool big)
{
	ColorBrush->SetColor(Clr);
	if (!big)pRenderTarget->DrawText(Str, wcslen(Str), TextFormat, { pos.x - wcslen(Str) * 5.8f / 2, pos.y - 7,FLT_MAX,FLT_MAX }, ColorBrush);
	else pRenderTarget->DrawText(Str, wcslen(Str), BigTextFormat, { pos.x, pos.y, FLT_MAX, FLT_MAX }, ColorBrush);
}

void gui_esp::String(const Vector2& pos, const wchar_t* Str, const D2D1::ColorF& Clr, bool big)
{
	ColorBrush->SetColor(Clr);
	if (!big)pRenderTarget->DrawText(Str, wcslen(Str), TextFormat, { pos.x , pos.y,FLT_MAX,FLT_MAX }, ColorBrush);
	else pRenderTarget->DrawText(Str, wcslen(Str), BigTextFormat, { pos.x, pos.y, FLT_MAX, FLT_MAX }, ColorBrush);
}

//WndProc function
void gui_esp::drawHealthBar(float Entity_x, float Entity_y, float height, float healthBarY, const D2D1::ColorF& color)
{
	ColorBrush->SetColor(color);
	pRenderTarget->FillRectangle(D2D1::RectF(Entity_x + 5, Entity_y + height, Entity_x, healthBarY), ColorBrush);
}

void gui_esp::drawAll() {
	gui_esp::init_window_Esp();
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT && globals::run_render)
	{
		


		if (PeekMessage(&msg, windowEsp, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		if (GetForegroundWindow() == m_gui::gameHWND || GetForegroundWindow() == gui_menu::windowMenu)
		{
			gui_esp::begin_draw_esp();

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
				drawHealthBar(Entity_x, Entity_y, height, healthBarY, D2D1::ColorF(0, 1, 0));


				std::string healthText = std::to_string(health);
				healthText += "%";
				gui_esp::String(Vector2(Entity_x - 5, healthBarY), std::wstring(healthText.begin(), healthText.end()).c_str(), D2D1::ColorF(D2D1::ColorF::White, 1.0f), false);

				std::string name = p.name;
				gui_esp::String(Vector2(Entity_x + Entity_w, Entity_y + 10), std::wstring(name.begin(), name.end()).c_str(), D2D1::ColorF(D2D1::ColorF::White, 1.0f), false);


				if (Driver::rpm<std::int32_t>(currEnt + offset::m_bSpottedByMask) & (1 << localPlayerId)) {

					gui_esp::rect(Entity_x, Entity_y, Entity_w, height, D2D1::ColorF(D2D1::ColorF::Blue, 0.5f));
				}
				else {
					gui_esp::rect(Entity_x, Entity_y, Entity_w, height, D2D1::ColorF(D2D1::ColorF::Red, 0.5f));

				}


				

			}
			gui_esp::end_draw_esp();
		}
		else {
			gui_esp::clear_window();
		}
		
		Sleep(1);
	}
	
	gui_esp::shutdown();





	
}
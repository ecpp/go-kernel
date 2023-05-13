#include "../../globals.h"
#include "gui_esp.h"
#include "../utils/driver.h"
#include "../utils/vector.h"
#include "gui_menu.h"
#include "main_gui.h"
#include "../utils/obfuscate.h"



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
	wcEsp.lpszClassName = AY_OBFUSCATE("E5P Window");

	RegisterClass(&wcEsp);

	windowEsp = CreateWindowEx
	(
		WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
		AY_OBFUSCATE("E5P Window"),
		AY_OBFUSCATE("Hack"),
		WS_POPUP,
		0,
		0,
		globals::screen::width,
		globals::screen::height,
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
		MessageBox(0, AY_OBFUSCATE("[GuiEngine ESP] init_render error"), AY_OBFUSCATE("ERROR"), MB_OK | MB_ICONERROR);
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
		MessageBox(0, AY_OBFUSCATE("D2D1CreateFactory"), AY_OBFUSCATE("ERROR"), MB_OK | MB_ICONERROR);
		std::cout << AY_OBFUSCATE("D2D1CreateFactory error") << std::endl;
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
		MessageBox(0, AY_OBFUSCATE("GuiEngine create_canvas ] !pRenderTarget"), AY_OBFUSCATE("ERROR"), MB_OK | MB_ICONERROR);
		std::cout << AY_OBFUSCATE("cant create canvas") << std::endl;
	}



	pRenderTarget->SetDpi(96, 96);
	pRenderTarget->CreateSolidColorBrush(color_brush, &ColorBrush);
}

void gui_esp::begin_draw_esp()
{
	if (!pRenderTarget)
	{
		std::cout << AY_OBFUSCATE("Error pRenderTarget") << std::endl;

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
			if (globals::isEsp == false)
				continue;

			//create empty circle
			gui_esp::Circle({ float(globals::screen::width) / 2, float(globals::screen::height) / 2 }, D2D1::ColorF(D2D1::ColorF::Black), globals::aimFov * 7, 1.0f);

			DWORD localplayer = Driver::rpm<DWORD>(globals::client + offset::dwLocalPlayer);
			DWORD localTeam = Driver::rpm<DWORD>(localplayer + offset::m_iTeamNum);//fixed

			Vector3 punchAngle = Driver::rpm<Vector3>(localplayer + offset::m_aimPunchAngle);

			const auto clientState = Driver::rpm<std::uint32_t>(globals::engine + offset::dwClientState);

			const auto userInfoTable = Driver::rpm<std::uint32_t>(clientState + offset::dwClientState_PlayerInfo);



			const auto localPlayerId =
				Driver::rpm<std::int32_t>(clientState + offset::dwClientState_GetLocalPlayer);


			punchAngle.x = punchAngle.x * 12; punchAngle.y = punchAngle.y * 12;
			float x = globals::screen::width / 2 - punchAngle.y;
			float y = globals::screen::height / 2 + punchAngle.x;


			esp::MAT4X4 viewMatrix = Driver::rpm<esp::MAT4X4>(globals::client + offset::dwViewMatrix);

			for (size_t i = 0; i < 32; i++)
			{


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
				healthText += AY_OBFUSCATE("%");
				gui_esp::String(Vector2(Entity_x - 5, healthBarY), std::wstring(healthText.begin(), healthText.end()).c_str(), D2D1::ColorF(D2D1::ColorF::White, 1.0f), false);

				std::string name = p.name;
				gui_esp::String(Vector2(Entity_x + Entity_w, Entity_y + 10), std::wstring(name.begin(), name.end()).c_str(), D2D1::ColorF(D2D1::ColorF::White, 1.0f), false);

				/*std::string weapon = Driver::rpm<char*>(Driver::rpm<DWORD>(currEnt + offset::m_hActiveWeapon) + offset::m_iItemDefinitionIndex);
				std::cout << weapon << std::endl;*/
				//gui_esp::String(Vector2(Entity_x + Entity_w, Entity_y + 20), std::wstring(weapon.begin(), weapon.end()).c_str(), D2D1::ColorF(D2D1::ColorF::White, 1.0f), false);
				//
				////draw head box
				//gui_esp::rect(headScreen.x - 1, headScreen.y - 1, 2, 2, D2D1::ColorF(D2D1::ColorF::Green, 1.0f));




				//check if spotted by mask
				if (Driver::rpm<std::int32_t>(currEnt + offset::m_bSpottedByMask)) {

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
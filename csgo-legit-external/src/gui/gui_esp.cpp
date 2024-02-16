#include "../../globals.h"
#include "gui_esp.h"
#include "../utils/driver.h"
#include "../utils/vector.h"
#include "gui_menu.h"
#include "main_gui.h"
#include "../utils/obfuscate.h"
#include "../hacks/esp.h"



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

void gui_esp::MovingCross(const Vector2& start, const Vector2& end, const D2D1::ColorF& clr, float thick)
{
	ColorBrush->SetColor(clr);
	pRenderTarget->DrawLine({ start.x, start.y }, { end.x, end.y }, ColorBrush, thick);
	pRenderTarget->DrawLine({ end.y, end.x }, { start.y, start.x }, ColorBrush, thick);
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

			int playerIndex = 0;
			uintptr_t list_entry;

			/**
			* Loop through all the players in the entity list
			*
			* (This could have been done by getting a entity list count from the engine, but I'm too lazy to do that)
			**/
			for (auto player = g_game.players.begin(); player < g_game.players.end(); player++) {
				const Vector3 screenPos = g_game.world_to_screen(&player->origin);
				const Vector3 screenHead = g_game.world_to_screen(&player->head);
				if (screenPos.z >= 0.01f) {
					Vector3 head;
					head.x = player->origin.x;
					head.y = player->origin.y;
					head.z = player->origin.z + 75.f;
					esp::MAT4X4 viewMatrix = Driver::rpm<esp::MAT4X4>(globals::client + offset::dwViewMatrix);
					Vector2 screenpos = esp::WorldToScreen(head, viewMatrix);
					Vector2 screenhead = esp::WorldToScreen(player->origin, viewMatrix);;

					float height = screenpos.y - screenhead.y;
					float width = height / 2.4f;

					float distance = g_game.localOrigin.calculate_distance(player->origin);
					int roundedDistance = std::round(distance / 10.f);

					
					gui_esp::rect(screenhead.x - width / 2, screenhead.y, width, height, D2D1::ColorF(D2D1::ColorF::Red), 1.0f);
					int health = player->health;
					std::cout << health << std::endl;
					int healthBar = (int)(height * (health / 100.f));
					int healthBarY = screenhead.y + height - healthBar;
					std::string healthText = std::to_string(health);
					healthText += "%";
					gui_esp::String(Vector2(screenhead.x - 5, screenhead.y), std::wstring(healthText.begin(), healthText.end()).c_str(), D2D1::ColorF(D2D1::ColorF::White, 1.0f), false);
					//gui_esp::drawHealthBar(screenhead.x + 20, screenhead.y, height, healthBarY, D2D1::ColorF(D2D1::ColorF::Green, 1.0f));
					//render::DrawBorderBox(
					//	g::hdcBuffer,
					//	screenHead.x - (width / 2 + 10),
					//	screenHead.y + (height * (100 - player->armor) / 100),
					//	2,
					//	height - (height * (100 - player->armor) / 100),
					//	RGB(0, 185, 255)
					//);

					//render::DrawBorderBox(
					//	g::hdcBuffer,
					//	screenHead.x - (width / 2 + 5),
					//	screenHead.y + (height * (100 - player->health) / 100),
					//	2,
					//	height - (height * (100 - player->health) / 100),
					//	RGB(
					//		(255 - player->health),
					//		(55 + player->health * 2),
					//		75
					//	)
					//);

					//render::RenderText(
					//	g::hdcBuffer,
					//	screenHead.x + (width / 2 + 5),
					//	screenHead.y,
					//	player->name.c_str(),
					//	config::esp_name_color,
					//	10
					//);

					///**
					//* I know is not the best way but a simple way to not saturate the screen with a ton of information
					//*/
					//if (roundedDistance > config::flag_render_distance)
					//	continue;

					//render::RenderText(
					//	g::hdcBuffer,
					//	screenHead.x + (width / 2 + 5),
					//	screenHead.y + 10,
					//	(std::to_string(player->health) + "hp").c_str(),
					//	RGB(
					//		(255 - player->health),
					//		(55 + player->health * 2),
					//		75
					//	),
					//	10
					//);

					//render::RenderText(
					//	g::hdcBuffer,
					//	screenHead.x + (width / 2 + 5),
					//	screenHead.y + 20,
					//	(std::to_string(player->armor) + "armor").c_str(),
					//	RGB(
					//		(255 - player->armor),
					//		(55 + player->armor * 2),
					//		75
					//	),
					//	10
					//);

					//if (config::show_extra_flags)
					//{
					//	render::RenderText(
					//		g::hdcBuffer,
					//		screenHead.x + (width / 2 + 5),
					//		screenHead.y + 30,
					//		player->weapon.c_str(),
					//		config::esp_distance_color,
					//		10
					//	);

					//	/*render::RenderText(
					//		g::hdcBuffer,
					//		screenHead.x + (width / 2 + 5),
					//		screenHead.y + 40,
					//		(std::to_string(roundedDistance) + "m away").c_str(),
					//		config::esp_distance_color,
					//		10
					//	);*/

					//	/*render::RenderText(
					//		g::hdcBuffer,
					//		screenHead.x + (width / 2 + 5),
					//		screenHead.y + 50,
					//		("$" + std::to_string(player->money)).c_str(),
					//		RGB(0, 125, 0),
					//		10
					//	);*/

					//	/*if (player->flashAlpha > 100)
					//	{
					//		render::RenderText(
					//			g::hdcBuffer,
					//			screenHead.x + (width / 2 + 5),
					//			screenHead.y + 60,
					//			"Player is flashed",
					//			config::esp_distance_color,
					//			10
					//		);
					//	}*/

					//	/*if (player->is_defusing)
					//	{
					//		const std::string defuText = "Player is defusing";
					//		render::RenderText(
					//			g::hdcBuffer,
					//			screenHead.x + (width / 2 + 5),
					//			screenHead.y + 60,
					//			defuText.c_str(),
					//			config::esp_distance_color,
					//			10
					//		);
					//	}*/
					//}
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
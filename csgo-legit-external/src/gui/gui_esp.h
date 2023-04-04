#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <d3d11.h>
#include <dwmapi.h>
#include <Windows.h>
#include "../utils/vector.h"
#include <d2d1.h>
#include <dwmapi.h>
#include <dwrite_1.h>


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "gui_menu.h"
#include "gui_menu.h"

#pragma comment (lib, "dwmapi.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")



namespace gui_esp {
	/*inline ID3D11Device* deviceEsp = { nullptr };
	inline ID3D11DeviceContext* contextEsp = { nullptr };
	inline IDXGISwapChain* swapChainEsp = { nullptr };
	inline ID3D11RenderTargetView* renderTargetEsp = { nullptr };
	inline D3D_FEATURE_LEVEL featureLevelEsp = {};
	inline WNDCLASSEXW wcexEsp = {};
	inline HWND windowEsp = { nullptr };*/

	/*void initWindow();
	void initDevice();
	void initRender();
	void shutdown();*/


	inline HWND windowEsp = { nullptr };
	bool init_window_Esp();

	bool init_render();
	void create_canvas();
	void cleanup_canvas();

	void begin_draw_esp();
	void end_draw_esp();
	void clear_window();

	void shutdown();
	void drawAll();

	//primitives
	void rect(float x, float y, float h, float w, const D2D1::ColorF& Clr, float Thick = 1.5f);
	void fill_rect(float x, float y, float w, float h, const D2D1::ColorF& Clr);
	void Line(const Vector2& Start, const Vector2& End, const D2D1::ColorF& Clr, float Thick = 1.5f);
	void Circle(const Vector2& Start, const D2D1::ColorF& Clr, float Rad, float Thick = 1.5f);
	void CenterString(const Vector2& pos, const wchar_t* Str, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White), bool big = false);
	void String(const Vector2& pos, const wchar_t* Str, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White), bool big = false);
	void Crosshair(const int& screenWidth, const int& screenHeight, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White), const float& thick = 2);
	void drawHealthBar(float Entity_x, float Entity_y, float height, float healthBarY, const D2D1::ColorF& color);
	
}

namespace esp {
	typedef struct _MAT4X4
	{
		float c[4][4];
	}MAT4X4, * PMAT4X4;

	typedef struct _MAT3X4
	{
		float c[3][4];
	}MAT3X4, * PMAT3X4;

	inline Vector2 WorldToScreen(Vector3& pos, MAT4X4& g_viewMatrix)
	{
		float _x = g_viewMatrix.c[0][0] * pos.x + g_viewMatrix.c[0][1] * pos.y + g_viewMatrix.c[0][2] * pos.z + g_viewMatrix.c[0][3];
		float _y = g_viewMatrix.c[1][0] * pos.x + g_viewMatrix.c[1][1] * pos.y + g_viewMatrix.c[1][2] * pos.z + g_viewMatrix.c[1][3];

		float w = g_viewMatrix.c[3][0] * pos.x + g_viewMatrix.c[3][1] * pos.y + g_viewMatrix.c[3][2] * pos.z + g_viewMatrix.c[3][3];

		if (w < 0.01f)
			return { 0,0 };


		float inv_w = 1.f / w;
		_x *= inv_w;
		_y *= inv_w;

		float x = 1920 * .5f;
		float y = 1080 * .5f;

		x += 0.5f * _x * 1920 + 0.5f;
		y -= 0.5f * _y * 1080 + 0.5f;

		return { x,y };
	}

	class player_info_t
	{
	private:
		char __pad[0x10];
	public:
		char name[32];
	};
}


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
#include "gui_menu.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "gui_esp.h"

#pragma comment (lib, "dwmapi.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

extern ID3D11Device* g_pd3dDevice;
extern IDXGISwapChain* g_pSwapChain;

extern ID2D1HwndRenderTarget* pRenderTarget;

namespace m_gui {
	inline LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	inline HWND gameHWND = NULL;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI m_gui::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if (hWnd == gui_menu::windowMenu && hWnd != NULL)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		switch (msg)
		{
		case WM_SIZE:
			if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
			{
				gui_menu::cleanup_render_target();
				g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
				gui_menu::create_render_target();
			}
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	else if (hWnd == gui_esp::windowEsp && hWnd != NULL)
	{
		switch (msg)
		{
		case WM_SIZE:
			if (pRenderTarget != NULL)pRenderTarget->Resize(D2D1::SizeU((UINT)LOWORD(lParam), (UINT)HIWORD(lParam)));
			//std::cout <<"Buffers: "<< (UINT)LOWORD(lParam) << " " << (UINT)HIWORD(lParam) << std::endl;
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);

	}
	else return DefWindowProc(hWnd, msg, wParam, lParam);

}
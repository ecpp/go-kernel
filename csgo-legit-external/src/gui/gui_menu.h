#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <d3d11.h>
#include <dwmapi.h>
#include <Windows.h>
#include "../utils/vector.h"

#pragma comment (lib, "dwmapi.lib")
#pragma comment (lib, "d3d11.lib")

namespace gui_menu {
	inline m_gui::Hotkey aimbot_hotkey = { &globals::aimKey };
	inline m_gui::Hotkey aimassist_hotkey = { &globals::aimAssitKey };
	inline m_gui::Hotkey trigger_hotkey = { &globals::triggerKey };


	inline ID3D11Device* deviceMenu = { nullptr };
	inline ID3D11DeviceContext* contextMenu = { nullptr };
	inline IDXGISwapChain* swapChainMenu = { nullptr };
	inline ID3D11RenderTargetView* renderTargetMenu = { nullptr };
	inline D3D_FEATURE_LEVEL featureLevelMenu = {};
	inline WNDCLASSEXW wcexMenu = {};
	inline HWND windowMenu = { nullptr };

	void initWindow();
	//void initDevice();
	void initRender();
	void shutdown();

	bool create_device_D3D(HWND hwnd);
	void cleanup_device_D3D();
	void create_render_target();
	void cleanup_render_target();


}
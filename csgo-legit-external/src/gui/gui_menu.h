#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <d3d11.h>
#include <dwmapi.h>
#include <Windows.h>
#include "../utils/vector.h"
#include <imgui/imgui_internal.h>
#include <string>
#include "../../globals.h"
#include <unordered_map>

#pragma comment (lib, "dwmapi.lib")
#pragma comment (lib, "d3d11.lib")

namespace gui_menu {
<<<<<<< Updated upstream
	inline m_gui::Hotkey aimbot_hotkey = { &globals::aimKey };
	inline m_gui::Hotkey aimassist_hotkey = { &globals::aimAssitKey };
	inline m_gui::Hotkey trigger_hotkey = { &globals::triggerKey };


=======
	
>>>>>>> Stashed changes
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
	void startMenu();

	struct Hotkey {
		int* key;
		bool waiting_for_key = false;
		bool prev_key_state[512] = {};

		std::unordered_map<int, int> MouseToButtonMap = {
			{0, ImGuiMouseButton_Left},
			{1, ImGuiMouseButton_Right},
			{2, ImGuiMouseButton_Middle},
			// Add more mappings as necessary
			};

		void render() {
			ImGuiIO& io = ImGui::GetIO();
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			if (ImGui::Button(waiting_for_key ? "..." : ("Bind: " + std::string(KeyNames[*key])).c_str()))
			{
				waiting_for_key = true;
			}

			if (waiting_for_key) {
				int new_key = -1;
				for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
				{
					if (io.KeysDown[i] && i != ImGuiKey_Tab)
					{
						new_key = i;
						break;
					}
				}

				if (new_key == -1) {
					for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {
						// Get the platform-specific mouse button code
						if (io.MouseDown[i]) {
							std::cout << "Mouse button " << i << " pressed" << std::endl;
							new_key = i+1;
							break;
						}
					}
				}

				// If we're waiting for a new key input and a new key was pressed, update the hotkey and stop waiting
				if (waiting_for_key && new_key != -1)
				{
					*key = new_key;
					waiting_for_key = false;
				}

				// Get the string representation of the hotkey
				const char* key_name = "None";
				if (*key >= 0 && *key < IM_ARRAYSIZE(KeyCodes))
				{
					key_name = KeyNames[*key];
				}
			}

			
		}
	};

	inline Hotkey aimbot_hotkey = { &globals::aimKey };
	inline Hotkey aimassist_hotkey = { &globals::aimAssitKey };
	inline Hotkey trigger_hotkey = { &globals::triggerKey };

}
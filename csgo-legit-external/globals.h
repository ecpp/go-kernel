#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "src/api/api.h"
#include <vector>
#include <thread>
#include <array>
#include <chrono>
#include <iostream>
#include "src/utils/obfuscate.h"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"     /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[1;32m"     /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"     /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */F
#define BOLDBLUE    "\033[1m\033[34m"     /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

namespace offset
{
    inline ptrdiff_t dwClientState, dwClientState_PlayerInfo,
        dwClientState_GetLocalPlayer, dwClientState_MapDirectory,
        dwClientState_MaxPlayer,
        dwGameDir, dwClientState_Map, dwForceJump, dwClientState_ViewAngles;

    inline std::ptrdiff_t dwLocalPlayer = 24281784;
    inline std::ptrdiff_t dwLocalPlayerController = 26225032;
    inline std::ptrdiff_t dwEntityList = 25899032;
    inline std::ptrdiff_t dwViewMatrix = 26288928;
    inline std::ptrdiff_t dwPlantedC4 = 26313272;

    inline std::ptrdiff_t m_ArmorValue = 5488;
    inline std::ptrdiff_t m_bIsDefusing = 5128;
    inline std::ptrdiff_t m_flC4Blow = 3848;
    inline std::ptrdiff_t m_flFlashOverlayAlpha = 5312;
    inline std::ptrdiff_t m_flNextBeep = 3844;
    inline std::ptrdiff_t m_flTimerLength = 3856;
    inline std::ptrdiff_t m_iAccount = 64;
    inline std::ptrdiff_t m_pClippingWeapon = 4872;
    inline std::ptrdiff_t m_pGameSceneNode = 792;
    inline std::ptrdiff_t m_pInGameMoneyServices = 1784;
    inline std::ptrdiff_t m_szName = 3104;
    inline std::ptrdiff_t m_iHealth = 820;
    inline std::ptrdiff_t m_iTeamNum = 971;
    inline std::ptrdiff_t m_vecOrigin = 1360;
    inline std::ptrdiff_t m_vOldOrigin = 4732;
    inline std::ptrdiff_t m_vecAbsOrigin = 200;
    inline std::ptrdiff_t m_bPawnIsAlive = 2028;
    inline std::ptrdiff_t m_hPlayerPawn = 2020;
    inline std::ptrdiff_t m_sSanitizedPlayerName = 1864;
    
    namespace Signatures
    {
        const std::string EntityList = "48 8B 0D ?? ?? ?? ?? 48 89 7C 24 ?? 8B FA C1";
        const std::string LocalPlayerController = "48 8B 05 ?? ?? ?? ?? 48 85 C0 74 4F";
        const std::string ViewAngles = "48 8B 0D ?? ?? ?? ?? 48 8B 01 48 FF 60 30";
        const std::string ViewMatrix = "48 8D 0D ?? ?? ?? ?? 48 C1 E0 06";
        const std::string LocalPlayerPawn = "48 8D 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC 48 83 EC ?? 8B 0D";
        const std::string ForceJump = "48 8B 05 ?? ?? ?? ?? 48 8D 1D ?? ?? ?? ?? 48 89 45";
    }

}

namespace globals {
    inline auto windowName = "Counter-Strike 2";

    inline bool automatic_update = false;
    inline bool team_esp = true;
    inline float render_distance = -1.f;
    inline int flag_render_distance = 200;
    inline bool show_box_esp = true;
    inline bool show_skeleton_esp = true;
    inline bool show_head_tracker = true;
    inline bool show_extra_flags = true;

    inline bool isAim = false;
    inline auto aimKey = 0x01;
    inline auto aimFov = 0.f;
    inline auto aimSmooth = 0.1f;

    inline bool isBunny = false;
    inline bool isRadar = false;

    inline bool isAimAssit = false;
    inline auto aimAssitKey = 0x01;

    inline bool isNoRecoil = false;

    inline bool isTrigger = false;
    inline auto triggerKey = 0x03;

    inline bool isEsp = false;



    inline bool isFlash = false;
    inline bool isMenu = false;
    inline bool isAutoAccept = false;
    inline bool isRecoilCross = false;

    inline bool runhax = true;
    inline auto legitAimSmooth = 0.1f;
    inline auto legitAimHeadChance = 60;

    inline HANDLE processID = NULL;
    inline std::uintptr_t client = 0;
    inline std::uintptr_t engine = 0;
    inline std::uintptr_t serverbrowser = 0;
    inline std::uintptr_t enginesize = 0;
    inline std::uintptr_t clientsize = 0;

    inline std::uintptr_t localPlayer = 0;
    inline std::uintptr_t localTeam = 0;
    inline std::uintptr_t localPlayerFlags = 0;
    inline std::uintptr_t clientState = 0;

    inline std::array<char, 0x80> mapDirectory = { 0 };
    inline std::array<char, 0x120> mapName = { 0 };
    inline std::array<char, 0x100> gameDirectory = { 0 };
    inline std::string fullPath = "";

    inline bool run_render = true;

    inline std::string hwid = "";
    inline std::string userToken = "";
    inline std::string version = "0.1";

    inline API api;

    namespace screen {
        inline auto width = GetSystemMetrics(SM_CXSCREEN);
        inline auto height = GetSystemMetrics(SM_CYSCREEN);
    }

    namespace esp {
        inline bool isBox = false;
        inline bool isName = false;
        inline bool isHealth = false;
        inline bool isWeapon = false;
        inline bool playerWeapon = false;
    }

    inline std::string obfuscate(const std::string& input) {
        std::string obfuscated = input;
        const char key = 'X';
        for (char& c : obfuscated) {
            c ^= key;
        }
        return obfuscated;
    }


}

namespace weapons {

    static const std::unordered_map<short, std::string> weaponDictionary = {
    { 1, "Desert Eagle" },
    { 2, "Dual Berettas" },
    { 3, "Five-SeveN" },
    { 4, "Glock-18" },
    { 7, "AK-47" },
    { 8, "AUG" },
    { 9, "AWP" },
    { 10, "FAMAS" },
    { 11, "G3SG1" },
    { 13, "Galil AR" },
    { 14, "M249" },
    { 16, "M4A4" },
    { 17, "MAC-10" },
    { 19, "P90" },
    { 23, "MP5-SD" },
    { 24, "UMP-45" },
    { 25, "XM1014" },
    { 26, "PP-Bizon" },
    { 27, "MAG-7" },
    { 28, "Negev" },
    { 29, "Sawed-Off" },
    { 30, "Tec-9" },
    { 32, "P2000" },
    { 33, "MP7" },
    { 34, "MP9" },
    { 35, "Nova" },
    { 36, "P250" },
    { 38, "SCAR-20" },
    { 39, "SG 553" },
    { 40, "SSG 08" },
    { 41, "Knife" },
    { 42, "Flashbang" },
    { 43, "HE Grenade" },
    { 44, "Smoke Grenade" },
    { 45, "Molotov" },
    { 46, "Decoy Grenade" },
    { 47, "Incendiary Grenade" },
    { 48, "C4 Explosive" },
    { 49, "Knife" },
    { 59, "Knife" },
    { 60, "M4A1-S" },
    { 61, "USP-S" },
    { 63, "CZ75-Auto" },
    { 64, "R8 Revolver" },
    { 500, "Bayonet" },
    { 505, "Flip Knife" },
    { 506, "Gut Knife" },
    { 507, "Karambit" },
    { 508, "M9 Bayonet" },
    { 509, "Huntsman Knife" },
    { 512, "Falchion Knife" },
    { 514, "Bowie Knife" },
    { 515, "Butterfly Knife" },
    { 516, "Shadow Daggers" },
    { 519, "Ursus Knife" },
    { 520, "Navaja Knife" },
    { 522, "Stiletto Knife" },
    { 523, "Talon Knife" },
    { 525, "Skeleton Knife" }
    };

    inline std::string getWeaponName(int id) {
        if (weaponDictionary.find(id) != weaponDictionary.end()) {
            return weaponDictionary.at(id);
        }
        return "Unknown";
    }
}


static const char* KeyNames[] = {
    "OFF",
    "VK_LBUTTON",
    "VK_RBUTTON",
    "VK_CANCEL",
    "VK_MBUTTON",
    "VK_XBUTTON1",
    "VK_XBUTTON2",
    "Unknown",
    "VK_BACK",
    "VK_TAB",
    "Unknown",
    "Unknown",
    "VK_CLEAR",
    "VK_RETURN",
    "Unknown",
    "Unknown",
    "VK_SHIFT",
    "VK_CONTROL",
    "VK_MENU",
    "VK_PAUSE",
    "VK_CAPITAL",
    "VK_KANA",
    "Unknown",
    "VK_JUNJA",
    "VK_FINAL",
    "VK_KANJI",
    "Unknown",
    "VK_ESCAPE",
    "VK_CONVERT",
    "VK_NONCONVERT",
    "VK_ACCEPT",
    "VK_MODECHANGE",
    "VK_SPACE",
    "VK_PRIOR",
    "VK_NEXT",
    "VK_END",
    "VK_HOME",
    "VK_LEFT",
    "VK_UP",
    "VK_RIGHT",
    "VK_DOWN",
    "VK_SELECT",
    "VK_PRINT",
    "VK_EXECUTE",
    "VK_SNAPSHOT",
    "VK_INSERT",
    "VK_DELETE",
    "VK_HELP",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "VK_LWIN",
    "VK_RWIN",
    "VK_APPS",
    "Unknown",
    "VK_SLEEP",
    "VK_NUMPAD0",
    "VK_NUMPAD1",
    "VK_NUMPAD2",
    "VK_NUMPAD3",
    "VK_NUMPAD4",
    "VK_NUMPAD5",
    "VK_NUMPAD6",
    "VK_NUMPAD7",
    "VK_NUMPAD8",
    "VK_NUMPAD9",
    "VK_MULTIPLY",
    "VK_ADD",
    "VK_SEPARATOR",
    "VK_SUBTRACT",
    "VK_DECIMAL",
    "VK_DIVIDE",
    "VK_F1",
    "VK_F2",
    "VK_F3",
    "VK_F4",
    "VK_F5",
    "VK_F6",
    "VK_F7",
    "VK_F8",
    "VK_F9",
    "VK_F10",
    "VK_F11",
    "VK_F12",
    "VK_F13",
    "VK_F14",
    "VK_F15",
    "VK_F16",
    "VK_F17",
    "VK_F18",
    "VK_F19",
    "VK_F20",
    "VK_F21",
    "VK_F22",
    "VK_F23",
    "VK_F24",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "VK_NUMLOCK",
    "VK_SCROLL",
    "VK_OEM_NEC_EQUAL",
    "VK_OEM_FJ_MASSHOU",
    "VK_OEM_FJ_TOUROKU",
    "VK_OEM_FJ_LOYA",
    "VK_OEM_FJ_ROYA",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "VK_LSHIFT",
    "VK_RSHIFT",
    "VK_LCONTROL",
    "VK_RCONTROL",
    "VK_LMENU",
    "VK_RMENU"
};
static const int KeyCodes[] = {
    0x0,  //Undefined
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07, //Undefined
    0x08,
    0x09,
    0x0A, //Reserved
    0x0B, //Reserved
    0x0C,
    0x0D,
    0x0E, //Undefined
    0x0F, //Undefined
    0x10,
    0x11,
    0x12,
    0x13,
    0x14,
    0x15,
    0x16, //IME On
    0x17,
    0x18,
    0x19,
    0x1A, //IME Off
    0x1B,
    0x1C,
    0x1D,
    0x1E,
    0x1F,
    0x20,
    0x21,
    0x22,
    0x23,
    0x24,
    0x25,
    0x26,
    0x27,
    0x28,
    0x29,
    0x2A,
    0x2B,
    0x2C,
    0x2D,
    0x2E,
    0x2F,
    0x30,
    0x31,
    0x32,
    0x33,
    0x34,
    0x35,
    0x36,
    0x37,
    0x38,
    0x39,
    0x3A, //Undefined
    0x3B, //Undefined
    0x3C, //Undefined
    0x3D, //Undefined
    0x3E, //Undefined
    0x3F, //Undefined
    0x40, //Undefined
    0x41,
    0x42,
    0x43,
    0x44,
    0x45,
    0x46,
    0x47,
    0x48,
    0x49,
    0x4A,
    0x4B,
    0x4C,
    0x4B,
    0x4E,
    0x4F,
    0x50,
    0x51,
    0x52,
    0x53,
    0x54,
    0x55,
    0x56,
    0x57,
    0x58,
    0x59,
    0x5A,
    0x5B,
    0x5C,
    0x5D,
    0x5E, //Rservered
    0x5F,
    0x60, //Numpad1
    0x61, //Numpad2
    0x62, //Numpad3
    0x63, //Numpad4
    0x64, //Numpad5
    0x65, //Numpad6
    0x66, //Numpad7
    0x67, //Numpad8
    0x68, //Numpad8
    0x69, //Numpad9
    0x6A,
    0x6B,
    0x6C,
    0x6D,
    0x6E,
    0x6F,
    0x70, //F1
    0x71, //F2
    0x72, //F3
    0x73, //F4
    0x74, //F5
    0x75, //F6
    0x76, //F7
    0x77, //F8
    0x78, //F9
    0x79, //F10
    0x7A, //F11
    0x7B, //F12
    0x7C, //F13
    0x7D, //F14
    0x7E, //F15
    0x7F, //F16
    0x80, //F17
    0x81, //F18
    0x82, //F19
    0x83, //F20
    0x84, //F21
    0x85, //F22
    0x86, //F23
    0x87, //F24
    0x88, //Unkown
    0x89, //Unkown
    0x8A, //Unkown
    0x8B, //Unkown
    0x8C, //Unkown
    0x8D, //Unkown
    0x8E, //Unkown
    0x8F, //Unkown
    0x90,
    0x91,
    0x92, //OEM Specific
    0x93, //OEM Specific
    0x94, //OEM Specific
    0x95, //OEM Specific
    0x96, //OEM Specific
    0x97, //Unkown
    0x98, //Unkown
    0x99, //Unkown
    0x9A, //Unkown
    0x9B, //Unkown
    0x9C, //Unkown
    0x9D, //Unkown
    0x9E, //Unkown 
    0x9F, //Unkown
    0xA0,
    0xA1,
    0xA2,
    0xA3,
    0xA4,
    0xA5
};

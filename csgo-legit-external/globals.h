#pragma once
#include <vector>
#include <thread>
#include <array>
#include <chrono>
#include <Windows.h>
#include <iostream>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

namespace offset
{
    constexpr ::std::ptrdiff_t cs_gamerules_data = 0x0;
    constexpr ::std::ptrdiff_t m_ArmorValue = 0x117CC;
    constexpr ::std::ptrdiff_t m_Collision = 0x320;
    constexpr ::std::ptrdiff_t m_CollisionGroup = 0x474;
    constexpr ::std::ptrdiff_t m_Local = 0x2FCC;
    constexpr ::std::ptrdiff_t m_MoveType = 0x25C;
    constexpr ::std::ptrdiff_t m_OriginalOwnerXuidHigh = 0x31D4;
    constexpr ::std::ptrdiff_t m_OriginalOwnerXuidLow = 0x31D0;
    constexpr ::std::ptrdiff_t m_SurvivalGameRuleDecisionTypes = 0x1328;
    constexpr ::std::ptrdiff_t m_SurvivalRules = 0xD00;
    constexpr ::std::ptrdiff_t m_aimPunchAngle = 0x303C;
    constexpr ::std::ptrdiff_t m_aimPunchAngleVel = 0x3048;
    constexpr ::std::ptrdiff_t m_angEyeAnglesX = 0x117D0;
    constexpr ::std::ptrdiff_t m_angEyeAnglesY = 0x117D4;
    constexpr ::std::ptrdiff_t m_bBombDefused = 0x29C0;
    constexpr ::std::ptrdiff_t m_bBombPlanted = 0x9A5;
    constexpr ::std::ptrdiff_t m_bBombTicking = 0x2990;
    constexpr ::std::ptrdiff_t m_bFreezePeriod = 0x20;
    constexpr ::std::ptrdiff_t m_bGunGameImmunity = 0x9990;
    constexpr ::std::ptrdiff_t m_bHasDefuser = 0x117DC;
    constexpr ::std::ptrdiff_t m_bHasHelmet = 0x117C0;
    constexpr ::std::ptrdiff_t m_bInReload = 0x32B5;
    constexpr ::std::ptrdiff_t m_bIsDefusing = 0x997C;
    constexpr ::std::ptrdiff_t m_bIsQueuedMatchmaking = 0x74;
    constexpr ::std::ptrdiff_t m_bIsScoped = 0x9974;
    constexpr ::std::ptrdiff_t m_bIsValveDS = 0x7C;
    constexpr ::std::ptrdiff_t m_bSpotted = 0x93D;
    constexpr ::std::ptrdiff_t m_bSpottedByMask = 0x980;
    constexpr ::std::ptrdiff_t m_bStartedArming = 0x3400;
    constexpr ::std::ptrdiff_t m_bUseCustomAutoExposureMax = 0x9D9;
    constexpr ::std::ptrdiff_t m_bUseCustomAutoExposureMin = 0x9D8;
    constexpr ::std::ptrdiff_t m_bUseCustomBloomScale = 0x9DA;
    constexpr ::std::ptrdiff_t m_clrRender = 0x70;
    constexpr ::std::ptrdiff_t m_dwBoneMatrix = 0x26A8;
    constexpr ::std::ptrdiff_t m_fAccuracyPenalty = 0x3340;
    constexpr ::std::ptrdiff_t m_fFlags = 0x104;
    constexpr ::std::ptrdiff_t m_flC4Blow = 0x29A0;
    constexpr ::std::ptrdiff_t m_flCustomAutoExposureMax = 0x9E0;
    constexpr ::std::ptrdiff_t m_flCustomAutoExposureMin = 0x9DC;
    constexpr ::std::ptrdiff_t m_flCustomBloomScale = 0x9E4;
    constexpr ::std::ptrdiff_t m_flDefuseCountDown = 0x29BC;
    constexpr ::std::ptrdiff_t m_flDefuseLength = 0x29B8;
    constexpr ::std::ptrdiff_t m_flFallbackWear = 0x31E0;
    constexpr ::std::ptrdiff_t m_flFlashDuration = 0x10470;
    constexpr ::std::ptrdiff_t m_flFlashMaxAlpha = 0x1046C;
    constexpr ::std::ptrdiff_t m_flLastBoneSetupTime = 0x2928;
    constexpr ::std::ptrdiff_t m_flLowerBodyYawTarget = 0x9ADC;
    constexpr ::std::ptrdiff_t m_flNextAttack = 0x2D80;
    constexpr ::std::ptrdiff_t m_flNextPrimaryAttack = 0x3248;
    constexpr ::std::ptrdiff_t m_flSimulationTime = 0x268;
    constexpr ::std::ptrdiff_t m_flTimerLength = 0x29A4;
    constexpr ::std::ptrdiff_t m_hActiveWeapon = 0x2F08;
    constexpr ::std::ptrdiff_t m_hBombDefuser = 0x29C4;
    constexpr ::std::ptrdiff_t m_hMyWeapons = 0x2E08;
    constexpr ::std::ptrdiff_t m_hObserverTarget = 0x339C;
    constexpr ::std::ptrdiff_t m_hOwner = 0x29DC;
    constexpr ::std::ptrdiff_t m_hOwnerEntity = 0x14C;
    constexpr ::std::ptrdiff_t m_hViewModel = 0x3308;
    constexpr ::std::ptrdiff_t m_iAccountID = 0x2FD8;
    constexpr ::std::ptrdiff_t m_iClip1 = 0x3274;
    constexpr ::std::ptrdiff_t m_iCompetitiveRanking = 0x1A84;
    constexpr ::std::ptrdiff_t m_iCompetitiveWins = 0x1B88;
    constexpr ::std::ptrdiff_t m_iCrosshairId = 0x11838;
    constexpr ::std::ptrdiff_t m_iDefaultFOV = 0x333C;
    constexpr ::std::ptrdiff_t m_iEntityQuality = 0x2FBC;
    constexpr ::std::ptrdiff_t m_iFOV = 0x31F4;
    constexpr ::std::ptrdiff_t m_iFOVStart = 0x31F8;
    constexpr ::std::ptrdiff_t m_iGlowIndex = 0x10488;
    constexpr ::std::ptrdiff_t m_iHealth = 0x100;
    constexpr ::std::ptrdiff_t m_iItemDefinitionIndex = 0x2FBA;
    constexpr ::std::ptrdiff_t m_iItemIDHigh = 0x2FD0;
    constexpr ::std::ptrdiff_t m_iMostRecentModelBoneCounter = 0x2690;
    constexpr ::std::ptrdiff_t m_iObserverMode = 0x3388;
    constexpr ::std::ptrdiff_t m_iShotsFired = 0x103E0;
    constexpr ::std::ptrdiff_t m_iState = 0x3268;
    constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
    constexpr ::std::ptrdiff_t m_lifeState = 0x25F;
    constexpr ::std::ptrdiff_t m_nBombSite = 0x2994;
    constexpr ::std::ptrdiff_t m_nFallbackPaintKit = 0x31D8;
    constexpr ::std::ptrdiff_t m_nFallbackSeed = 0x31DC;
    constexpr ::std::ptrdiff_t m_nFallbackStatTrak = 0x31E4;
    constexpr ::std::ptrdiff_t m_nForceBone = 0x268C;
    constexpr ::std::ptrdiff_t m_nModelIndex = 0x258;
    constexpr ::std::ptrdiff_t m_nTickBase = 0x3440;
    constexpr ::std::ptrdiff_t m_nViewModelIndex = 0x29D0;
    constexpr ::std::ptrdiff_t m_rgflCoordinateFrame = 0x444;
    constexpr ::std::ptrdiff_t m_szCustomName = 0x304C;
    constexpr ::std::ptrdiff_t m_szLastPlaceName = 0x35C4;
    constexpr ::std::ptrdiff_t m_thirdPersonViewAngles = 0x31E8;
    constexpr ::std::ptrdiff_t m_vecOrigin = 0x138;
    constexpr ::std::ptrdiff_t m_vecVelocity = 0x114;
    constexpr ::std::ptrdiff_t m_vecViewOffset = 0x108;
    constexpr ::std::ptrdiff_t m_viewPunchAngle = 0x3030;
    constexpr ::std::ptrdiff_t m_zoomLevel = 0x33E0;
    constexpr ::std::ptrdiff_t anim_overlays = 0x2990;
    constexpr ::std::ptrdiff_t clientstate_choked_commands = 0x4D30;
    constexpr ::std::ptrdiff_t clientstate_delta_ticks = 0x174;
    constexpr ::std::ptrdiff_t clientstate_last_outgoing_command = 0x4D2C;
    constexpr ::std::ptrdiff_t clientstate_net_channel = 0x9C;
    constexpr ::std::ptrdiff_t convar_name_hash_table = 0x301A0;
    constexpr ::std::ptrdiff_t dwClientState = 0x59F19C;
    constexpr ::std::ptrdiff_t dwClientState_GetLocalPlayer = 0x180;
    constexpr ::std::ptrdiff_t dwClientState_IsHLTV = 0x4D48;
    constexpr ::std::ptrdiff_t dwClientState_Map = 0x28C;
    constexpr ::std::ptrdiff_t dwClientState_MapDirectory = 0x188;
    constexpr ::std::ptrdiff_t dwClientState_MaxPlayer = 0x388;
    constexpr ::std::ptrdiff_t dwClientState_PlayerInfo = 0x52C0;
    constexpr ::std::ptrdiff_t dwClientState_State = 0x108;
    constexpr ::std::ptrdiff_t dwClientState_ViewAngles = 0x4D90;
    constexpr ::std::ptrdiff_t dwEntityList = 0x4DFFFB4;
    constexpr ::std::ptrdiff_t dwForceAttack = 0x322DDEC;
    constexpr ::std::ptrdiff_t dwForceAttack2 = 0x322DDF8;
    constexpr ::std::ptrdiff_t dwForceBackward = 0x322DE40;
    constexpr ::std::ptrdiff_t dwForceForward = 0x322DE34;
    constexpr ::std::ptrdiff_t dwForceJump = 0x52BBD48;
    constexpr ::std::ptrdiff_t dwForceLeft = 0x322DE4C;
    constexpr ::std::ptrdiff_t dwForceRight = 0x322DDB0;
    constexpr ::std::ptrdiff_t dwGameDir = 0x63AD80;
    constexpr ::std::ptrdiff_t dwGameRulesProxy = 0x532F56C;
    constexpr ::std::ptrdiff_t dwGetAllClasses = 0xE0BF8C;
    constexpr ::std::ptrdiff_t dwGlobalVars = 0x59EE60;
    constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x535AA70;
    constexpr ::std::ptrdiff_t dwInput = 0x525D588;
    constexpr ::std::ptrdiff_t dwInterfaceLinkList = 0x99C224;
    constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDEA964;
    constexpr ::std::ptrdiff_t dwMouseEnable = 0x52391E8;
    constexpr ::std::ptrdiff_t dwMouseEnablePtr = 0x52391B8;
    constexpr ::std::ptrdiff_t dwPlayerResource = 0x322C160;
    constexpr ::std::ptrdiff_t dwRadarBase = 0x5236A8C;
    constexpr ::std::ptrdiff_t dwSensitivity = 0xDEEB80;
    constexpr ::std::ptrdiff_t dwSensitivityPtr = 0xDEEB80;
    constexpr ::std::ptrdiff_t dwSetClanTag = 0x8DA80;
    constexpr ::std::ptrdiff_t dwViewMatrix = 0x4DF0DE4;
    constexpr ::std::ptrdiff_t dwWeaponTable = 0x525E664;
    constexpr ::std::ptrdiff_t dwWeaponTableIndex = 0x326C;
    constexpr ::std::ptrdiff_t dwYawPtr = 0xDEE910;
    constexpr ::std::ptrdiff_t dwZoomSensitivityRatioPtr = 0xDF4380;
    constexpr ::std::ptrdiff_t dwbSendPackets = 0xDD2B2;
    constexpr ::std::ptrdiff_t dwppDirect3DDevice9 = 0xA62C0;
    constexpr ::std::ptrdiff_t find_hud_element = 0x35740090;
    constexpr ::std::ptrdiff_t force_update_spectator_glow = 0x3D96FA;
    constexpr ::std::ptrdiff_t interface_engine_cvar = 0x3FA9C;
    constexpr ::std::ptrdiff_t is_c4_owner = 0x3E6F10;
    constexpr ::std::ptrdiff_t m_bDormant = 0xED;
    constexpr ::std::ptrdiff_t m_bIsLocalPlayer = 0x3628;
    constexpr ::std::ptrdiff_t m_flSpawnTime = 0x103C0;
    constexpr ::std::ptrdiff_t m_pStudioHdr = 0x2950;
    constexpr ::std::ptrdiff_t m_pitchClassPtr = 0x52390E0;
    constexpr ::std::ptrdiff_t m_yawClassPtr = 0xDEE910;
    constexpr ::std::ptrdiff_t model_ambient_min = 0x5A1194;
    constexpr ::std::ptrdiff_t set_abs_angles = 0x1E76F0;
    constexpr ::std::ptrdiff_t set_abs_origin = 0x1E7530;

}

namespace globals {

	inline bool isAim = false;
    inline auto aimKey = 0x01;
    inline auto aimFov = 10.f;
    inline auto aimSmooth = 0.1f;

	inline bool isBunny = false;
	inline bool isRadar = false;

	inline bool isAimAssit = false;
    inline auto aimAssitKey = 0x02;

	inline bool isNoRecoil = false;

	inline bool isTrigger = false;
    inline auto triggerKey = 0x03;

	inline bool isEsp = false;


    inline bool isFlash = false;
	inline bool isMenu = false;
    inline bool isAutoAccept = false;
	
	inline bool runhax = true;
	inline auto legitAimSmooth = 0.1f;
	inline auto legitAimHeadChance = 60;

	inline HANDLE processID = NULL;
	inline std::uintptr_t client = 0;
	inline std::uintptr_t engine = 0;
	inline std::uintptr_t serverbrowser = 0;

	inline std::uintptr_t localPlayer = 0;
	inline std::int32_t localTeam = 0;
	inline std::uintptr_t localPlayerFlags = 0;
    inline std::int32_t clientState = 0;

    inline std::array<char, 0x80> mapDirectory = { 0 };
    inline std::array<char, 0x120> mapName = { 0 };
    inline std::array<char, 0x100> gameDirectory = { 0 };
    inline std::string fullPath = "";

	inline bool run_render = true;

    namespace screen {
        inline auto width = GetSystemMetrics(SM_CXSCREEN);
        inline auto height = GetSystemMetrics(SM_CYSCREEN);
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

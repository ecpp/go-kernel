#include "esp.h"
#include "../utils/driver.h"

std::map<std::string, int> boneMap = {
	{"head", 6},
	{"neck_0", 5},
	{"spine_1", 4},
	{"spine_2", 2},
	{"pelvis", 0},
	{"arm_upper_L", 8},
	{"arm_lower_L", 9},
	{"hand_L", 10},
	{"arm_upper_R", 13},
	{"arm_lower_R", 14},
	{"hand_R", 15},
	{"leg_upper_L", 22},
	{"leg_lower_L", 23},
	{"ankle_L", 24},
	{"leg_upper_R", 25},
	{"leg_lower_R", 26},
	{"ankle_R", 27}
};

// CC4
uintptr_t CC4::get_planted() {
	return Driver::rpm<uintptr_t>(globals::client + offset::dwPlantedC4);

}

uintptr_t CC4::get_node() {
	return Driver::rpm<uintptr_t>(get_planted() + offset::m_pGameSceneNode);
}

Vector3 CC4::get_origin() {
	return Driver::rpm<Vector3>(get_node() + offset::m_vecAbsOrigin);
}

void CGame::loop() {
	inGame = false;
	isC4Planted = false;
	localPlayer = Driver::rpm<uintptr_t>(globals::client + offset::dwLocalPlayerController);
	if (!localPlayer) return;
	
	localPlayerPawn = Driver::rpm<uintptr_t>(localPlayer + offset::m_hPlayerPawn);
	if (!localPlayerPawn) return;

	entity_list = Driver::rpm<uintptr_t>(globals::client + offset::dwEntityList);


	localList_entry2 = Driver::rpm<uintptr_t>(entity_list + 0x8 * ((localPlayerPawn & 0x7FFF) >> 9) + 16);
	localpCSPlayerPawn = Driver::rpm<uintptr_t>(localList_entry2 + 120 * (localPlayerPawn & 0x1FF));
	if (!localpCSPlayerPawn) return;

	view_matrix = Driver::rpm<view_matrix_t>(globals::client + offset::dwViewMatrix);

	localTeam = Driver::rpm<int>(localPlayer + offset::m_iTeamNum);
	localOrigin = Driver::rpm<Vector3>(localpCSPlayerPawn + offset::m_vOldOrigin);
	isC4Planted = Driver::rpm<bool>(globals::client + offset::dwPlantedC4 - 0x8);

	inGame = true;
	int playerIndex = 0;
	std::vector<CPlayer> list;
	CPlayer player;
	uintptr_t list_entry, list_entry2, playerPawn, playerMoneyServices, clippingWeapon, weaponData, playerNameData;
	while (true) {
		playerIndex++;
		list_entry = Driver::rpm<uintptr_t>(entity_list + (8 * (playerIndex & 0x7FFF) >> 9) + 16);
		if (!list_entry) break;

		player.entity = Driver::rpm<uintptr_t>(list_entry + 120 * (playerIndex & 0x1FF));
		if (!player.entity) continue;

		/**
		* Skip rendering your own character and teammates
		*
		* If you really want you can exclude your own character from the check but
		* since you are in the same team as yourself it will be excluded anyway
		**/
		player.team = Driver::rpm<int>(player.entity + offset::m_iTeamNum);
		if (globals::team_esp && (player.team == localTeam)) continue;

		playerPawn = Driver::rpm<std::uint32_t>(player.entity + offset::m_hPlayerPawn);

		list_entry2 = Driver::rpm<uintptr_t>(entity_list + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
		if (!list_entry2) continue;

		player.pCSPlayerPawn = Driver::rpm<uintptr_t>(list_entry2 + 120 * (playerPawn & 0x1FF));
		if (!player.pCSPlayerPawn) continue;

		player.health = Driver::rpm<int>(player.pCSPlayerPawn + offset::m_iHealth);
		player.armor = Driver::rpm<int>(player.pCSPlayerPawn + offset::m_ArmorValue);
		if (player.health <= 0 || player.health > 100) continue;

		if (globals::team_esp && (player.pCSPlayerPawn == localPlayer)) continue;

		/*
		* Unused for now, but for a vis check
		*
		* player.spottedState = Driver::rpm<uintptr_t>(player.pCSPlayerPawn + 0x1630);
		* player.is_spotted = Driver::rpm<DWORD_PTR>(player.spottedState + 0xC); // bSpottedByMask
		* player.is_spotted = Driver::rpm<bool>(player.spottedState + 0x8); // bSpotted
		*/

		playerNameData = Driver::rpm<uintptr_t>(player.entity + offset::m_sSanitizedPlayerName);
		char buffer[256];
		//Driver::rpm(playerNameData, buffer, sizeof(buffer));
		std::string name = std::string(buffer);
		player.name = name;

		player.gameSceneNode = Driver::rpm<uintptr_t>(player.pCSPlayerPawn + offset::m_pGameSceneNode);
		player.origin = Driver::rpm<Vector3>(player.pCSPlayerPawn + offset::m_vOldOrigin);
		player.head = { player.origin.x, player.origin.y, player.origin.z + 75.f };

		if (player.origin.x == localOrigin.x && player.origin.y == localOrigin.y && player.origin.z == localOrigin.z)
			continue;

		if (globals::render_distance != -1 && (localOrigin - player.origin).length2d() > globals::render_distance) continue;
		if (player.origin.x == 0 && player.origin.y == 0) continue;

		if (globals::show_skeleton_esp) {
			player.gameSceneNode = Driver::rpm<uint64_t>(player.pCSPlayerPawn + 0x310);
			player.boneArray = Driver::rpm<uint64_t>(player.gameSceneNode + 0x160 + 0x80);
			player.ReadBones();
		}

		if (globals::show_head_tracker && !globals::show_skeleton_esp) {
			player.gameSceneNode = Driver::rpm<uint64_t>(player.pCSPlayerPawn + 0x310);
			player.boneArray = Driver::rpm<uint64_t>(player.gameSceneNode + 0x160 + 0x80);
			player.ReadHead();
		}

		if (globals::show_extra_flags) {
			/*
			* Reading values for extra flags is now seperated from the other reads
			* This removes unnecessary memory reads, improving performance when not showing extra flags
			*/
			player.is_defusing = Driver::rpm<bool>(player.pCSPlayerPawn + offset::m_bIsDefusing);

			playerMoneyServices = Driver::rpm<uintptr_t>(player.entity + offset::m_pInGameMoneyServices);
			player.money = Driver::rpm<int32_t>(playerMoneyServices + offset::m_iAccount);

			player.flashAlpha = Driver::rpm<float>(player.pCSPlayerPawn + offset::m_flFlashOverlayAlpha);

			clippingWeapon = Driver::rpm<std::uint64_t>(player.pCSPlayerPawn + offset::m_pClippingWeapon);
			weaponData = Driver::rpm<std::uint64_t>(clippingWeapon + 0x360);
			weaponData = Driver::rpm<std::uint64_t>(weaponData + offset::m_szName);
			char buffer[MAX_PATH];
			//Driver::rpm_raw(weaponData, buffer, sizeof(buffer));
			std::string weaponName = std::string(buffer);
			if (weaponName.compare(0, 7, "weapon_") == 0)
				player.weapon = weaponName.substr(7, weaponName.length()); // Remove weapon_ prefix
			else
				player.weapon = "Invalid Weapon Name";
		}

		list.push_back(player);
	}

	players.clear();
	players.assign(list.begin(), list.end());
}

uintptr_t boneAddress;
Vector3 bonePosition;
int boneIndex;
void CPlayer::ReadHead() {
	boneAddress = boneArray + 6 * 32;
	bonePosition = Driver::rpm<Vector3>(boneAddress);
	bones.bonePositions["head"] = g_game.world_to_screen(&bonePosition);

}

void CPlayer::ReadBones() {
	for (const auto& entry : boneMap) {
		const std::string& boneName = entry.first;
		boneIndex = entry.second;
		boneAddress = boneArray + boneIndex * 32;
		bonePosition = Driver::rpm<Vector3>(boneAddress);
		bones.bonePositions[boneName] = g_game.world_to_screen(&bonePosition);
	}
}

Vector3 CGame::world_to_screen(Vector3* v) {
	float _x = view_matrix[0][0] * v->x + view_matrix[0][1] * v->y + view_matrix[0][2] * v->z + view_matrix[0][3];
	float _y = view_matrix[1][0] * v->x + view_matrix[1][1] * v->y + view_matrix[1][2] * v->z + view_matrix[1][3];

	float w = view_matrix[3][0] * v->x + view_matrix[3][1] * v->y + view_matrix[3][2] * v->z + view_matrix[3][3];

	float inv_w = 1.f / w;
	_x *= inv_w;
	_y *= inv_w;

	float x = game_bounds.right * .5f;
	float y = game_bounds.bottom * .5f;

	x += 0.5f * _x * game_bounds.right + 0.5f;
	y -= 0.5f * _y * game_bounds.bottom + 0.5f;

	return { x, y, w };
}

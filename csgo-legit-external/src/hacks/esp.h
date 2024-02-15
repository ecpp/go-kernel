#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include "../utils/vector.h"
#include <map>
#include <string>
#include <vector>


struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}

	float matrix[4][4];
};

class CBones {
public:
	std::map<std::string, Vector3> bonePositions;
};

class CC4 {
public:
	uintptr_t get_planted();
	uintptr_t get_node();
	Vector3 get_origin();
};

class CPlayer {
public:
	uintptr_t entity;
	int team;
	uintptr_t pCSPlayerPawn;
	uintptr_t gameSceneNode;
	uintptr_t boneArray;
	uintptr_t spottedState;
	int health;
	int armor;
	std::string name;
	Vector3 origin;
	Vector3 head;
	CBones bones;
	bool is_defusing;
	bool is_spotted;
	int32_t money;
	float flashAlpha;
	std::string weapon;
	void ReadBones();
	void ReadHead();
};

class CGame
{
public:
	RECT game_bounds;
	uintptr_t buildNumber;
	bool inGame;
	Vector3 localOrigin;
	bool isC4Planted;
	int localTeam;
	CC4 c4;
	std::vector<CPlayer> players = {};
	void loop();
	Vector3 world_to_screen(Vector3* v);
private:
	view_matrix_t view_matrix;
	uintptr_t entity_list;
	uintptr_t localPlayer;
	uintptr_t localpCSPlayerPawn;
	std::uint32_t localPlayerPawn;
	uintptr_t localList_entry2;
};

inline CGame g_game;
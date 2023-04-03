#pragma once
#include "../utils/vector.h"

namespace esp {
	typedef struct _MAT4X4
	{
		float c[4][4];
	}MAT4X4, * PMAT4X4;

	typedef struct _MAT3X4
	{
		float c[3][4];
	}MAT3X4, * PMAT3X4;

	Vector2 WorldToScreen(Vector3& pos, MAT4X4& g_viewMatrix)
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
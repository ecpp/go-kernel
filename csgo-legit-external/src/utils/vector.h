#pragma once
#include <numbers>
#include <cmath>
#include <math.h>

constexpr float M_PI = 3.14159265358979323846;

struct Vector3
{
	// constructor
	constexpr Vector3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }

	// operator overloads
	constexpr const Vector3& operator-(const Vector3& other) const noexcept
	{
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}

	constexpr const Vector3& operator+(const Vector3& other) const noexcept
	{
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	constexpr const Vector3& operator/(const float factor) const noexcept
	{
		return Vector3{ x / factor, y / factor, z / factor };
	}

	constexpr const Vector3& operator*(const float factor) const noexcept
	{
		return Vector3{ x * factor, y * factor, z * factor };
	}

	// utils
	static Vector3 CalculateAngle(
		const Vector3& localPosition,
		const Vector3& enemyPosition,
		const Vector3& viewAngles)  noexcept
	{
		return ((enemyPosition - localPosition).ToAngle() - viewAngles);
	}

	Vector3 ToVector2()
	{
		float pitch, yaw;
		float hypotenuse = sqrt(x * x + y * y + z * z);
		pitch = ((double)atan(z / hypotenuse) * 180.0) / M_PI;
		yaw = ((double)atan(y / x) * 180.0) / M_PI;
		if (x >= 0.0) {
			yaw += 180.0;
		}

		return { pitch, yaw };
	}

	constexpr const Vector3& ToAngle() const noexcept
	{
		return Vector3{
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f };
	}

	constexpr const bool IsZero() const noexcept
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}

	// struct data
	float x, y, z;
};

struct Vector2 {
	constexpr Vector2(
		const float x = 0.f,
		const float y = 0.f) noexcept :
		x(x), y(y) { }

	float x, y;

	void Normalize() noexcept
	{
		if (x > 89.0) {
			x -= 360.0;
		}
		if (x < -89.0) {
			x += 360.0;
		}
		if (y > 180.0) {
			y -= 360.0;
		}
		if (y < -180.0) {
			y += 360.0;
		}
	}


};

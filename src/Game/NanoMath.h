#pragma once

struct Vec2 final
{
	float x{ 0.0f };
	float y{ 0.0f };
};

struct Vec3 final
{
	float x{ 0.0f };
	float y{ 0.0f };
	float z{ 0.0f };
};

struct Vec4 final
{
	float x{ 0.0f };
	float y{ 0.0f };
	float z{ 0.0f };
	float w{ 0.0f };
};

struct Quat final
{
	float w{ 0.0f };
	float x{ 0.0f };
	float y{ 0.0f };
	float z{ 0.0f };
};

struct Mat3 final
{
	[[nodiscard]] constexpr float& operator[](size_t i) noexcept { return value[i]; }
	[[nodiscard]] constexpr const float& operator[](size_t i) const noexcept { return value[i]; }

	float value[3 * 3] = { 0.0f };
};

struct Mat4 final
{
	[[nodiscard]] constexpr float& operator[](size_t i) noexcept { return value[i]; }
	[[nodiscard]] constexpr const float& operator[](size_t i) const noexcept { return value[i]; }

	float value[4 * 4] = { 0.0f };
};
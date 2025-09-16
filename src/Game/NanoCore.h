#pragma once

void Print(const std::string& msg);
void Info(const std::string& msg);
void Warning(const std::string& msg);
void Debug(const std::string& msg);
void Error(const std::string& msg);
void Fatal(const std::string& msg);

struct Rect2D final
{
	Rect2D() = default;
	Rect2D(uint32_t _x, uint32_t _y, uint16_t w, uint16_t h) : x(_x), y(_y), width(w), height(h) {}
	Rect2D(uint32_t _x, uint32_t _y, uint32_t w, uint32_t h) : x(_x), y(_y), width(w), height(h) {}

	constexpr bool operator==(const Rect2D&) const noexcept = default;

	uint32_t x{ 0 };
	uint32_t y{ 0 };
	uint32_t width{ 0 };
	uint32_t height{ 0 };
};

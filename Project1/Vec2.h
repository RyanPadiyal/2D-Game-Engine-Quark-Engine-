#pragma once
#include <cmath>


class Vec2 {

public:
	float x, y;

	Vec2() {};
	Vec2(float xin, float yin) : x(xin), y(yin) {}

	Vec2 operator + (const Vec2& rhs)
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}

	Vec2 operator - (const Vec2& rhs)
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}

	Vec2 operator *(float scalar) 
	{
		return Vec2(x * scalar, y * scalar);
	}

	float length()
	{
		return sqrt(x * x + y * y);
	}

	Vec2 normalize()
	{
		float magnitude = sqrt(x * x + y * y);
		return Vec2(x / magnitude, y / magnitude);
	}

};



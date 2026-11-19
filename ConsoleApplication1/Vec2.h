#pragma once
#include <cmath> 

template<typename T>

class Vec2

{
public:
	
	T x = 0;
	T y = 0;
	Vec2 (){}
	Vec2(T xin, T yin) : x(xin), y(yin){}


	Vec2 operator+ (const Vec2& rhs) const
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}

	Vec2 operator -(const Vec2& rhs) const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}

	Vec2 operator *( T scalar) const
	{
		return Vec2(x*scalar, y *scalar);
	}

	Vec2 operator /(T scalar) const
	{
		return Vec2(x / scalar, y / scalar);
	}

	bool operator == (const Vec2& rhs) const
	{
		if (x == rhs.x && y == rhs.y)
		{
			return true;
		}
		else return false;
	}

	bool operator != (const Vec2& rhs) const
	{
		if (x != rhs.x || y != rhs.y)
		{
			return true;
		}
		else return false;
	}

	Vec2 operator += ( const Vec2& rhs)
	{
		 x += rhs.x;
		 y += rhs.y;

		 return *this;
	}

	Vec2 operator -= (const Vec2& rhs)
	{
		 x -= rhs.x;
		 y -= rhs.y;
		 return *this;
	}

	Vec2 operator *= (T scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	Vec2 operator /= (T scalar)
	{
		 x /= scalar;
		 y /= scalar;
		 return *this;
	}
	
	float length() const
	{
		return std::sqrt(x * x + y * y);
	}

	Vec2 normalize() const
	{
		T len = length();
		return Vec2(x / len, y / len);
	}

	float distance(Vec2& rhs) const
	{
		return (*this - rhs).length();

	}

};

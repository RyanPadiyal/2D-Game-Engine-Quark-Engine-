#pragma once
#include "Vec2.h"
#include <SFML/Graphics.hpp>


class Component
{
public:
	bool exists = false;
};


class CTransform : public Component
{
public:
	Vec2<float> position = { 0.0f,0.0f };
	Vec2<float> velocity = { 0.0f, 0.0f };
	float angle = 0;

	CTransform() = default;
	CTransform( Vec2<float> p, Vec2<float> v, float a) : position(p), velocity(v), angle(a){}
};

class CShape : public Component
{
public:
	sf::CircleShape circle;
	CShape() = default;
	CShape(float radius, size_t points, sf::Color fill, sf::Color outline, float thickness) :circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}
};

class CCollision : public Component
{
public:
	float radius = 0;
	CCollision() = default;
	CCollision(float r) : radius(r){}
};

class CScore : public Component
{
public:
	int score = 0;
	CScore() = default;
	CScore(int s) : score(s){}
};


class CInput : public Component
{
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;
	CInput() = default;

};

class CLifespan : public Component
{
public:
	int lifespan = 0;
	int remaining = 0;
	CLifespan() = default;
	CLifespan(int TL) : lifespan(TL), remaining(TL){}
};



#include <iostream>
#include "Vec2.h"

int main()

{
	Vec2 v1(100, 500);

	Vec2 v2 = v1.normalize();

	std::cout << "Hello" << v2.x << v2.y;
}
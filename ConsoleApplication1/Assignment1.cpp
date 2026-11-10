#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "imgui.h"
#include "imgui-SFML.h"

int main() 
{

	//DEC AND INITITALIZING THE WINDOW
	sf::RenderWindow window(sf::VideoMode(1280, 720), "GSX");
	window.setFramerateLimit(60);
	sf::Clock deltaClock;
	ImGui::SFML::Init(window);


	//SQUARE DECLARATION
	float rectShapeX = 100.f;
	float rectShapeY = 100.f;
	sf::RectangleShape rect(sf::Vector2f(rectShapeX,rectShapeY));
	sf::Vector2 startPosition(100.f, 200.f);
	rect.setPosition(startPosition);
	rect.setFillColor(sf::Color::Green);
	bool drawRect = true;
	float rectXVel = 3.f;
	float rectYVel = 3.f;
	bool stopRect = false;
	sf::Vector2f rectSize = rect.getSize();


	//FONT
	sf::Font font;

	if (!font.loadFromFile("D:\\Work\\C++\\ConsoleApplication1\\x64\\Debug\\Fonts\\ARIAL.TTF"))
	{
		std::cerr << "Font file not loaded or not found big G";
		ImGui::SFML::Shutdown();
		return 1;
	}

	
	sf::Text rectText("Rectangle", font, 20.f);
	sf::FloatRect bounds = rectText.getLocalBounds();
	rectText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	

	//MAIN LOOP
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);
			if (event.type == sf::Event::Closed())
			{

				window.close();
			}
		}

		//MOVING THE RECTANGLE

		if (startPosition.x < 0.f || startPosition.x>1180.f)
		{
			rectXVel *= -1.f;
		}

		if (startPosition.y < 0.f || startPosition.y>620.f)
		{
			rectYVel *= -1.f;
		}

		startPosition.x += rectXVel;
		startPosition.y += rectYVel;
		rect.setPosition(startPosition);
		rectText.setPosition(startPosition.x + rectShapeX / 2.f, startPosition.y + rectShapeY / 2.f);

		//Resetting THE RECTANGLE

		if (stopRect) 
		
		{
			startPosition.x = 590;
			startPosition.y = 270;
			rect.setPosition(startPosition);
			rectText.setPosition(startPosition.x + rectShapeX / 2.f, startPosition.y + rectShapeY / 2.f);
		}


		//IMGUI PORTION
		ImGui::SFML::Update(window, deltaClock.restart());
		ImGui::Begin("Hello");
		ImGui::Text("Welcome Ryan!!!!!!!!!!!!!!");
		ImGui::Checkbox("Draw Rectangle", &drawRect);
		ImGui::Checkbox("Reset Rectangle", &stopRect);
		ImGui::SliderFloat("Change Length",&rectShapeX, 0.f, 250.f);
		ImGui::SliderFloat("Change breadth", &rectShapeY, 0.f, 250.0f);
		rect.setSize(sf::Vector2f(rectShapeX, rectShapeY));
		rectText.setPosition(startPosition.x + rectShapeX / 2.f,startPosition.y + rectShapeY / 2.f);
		ImGui::End();


		window.clear();
		if(drawRect)
		{
			window.draw(rect);
		}

		window.draw(rectText);

		ImGui::SFML::Render(window);
		window.display();

	
		
	
	}

	ImGui::SFML::Shutdown();

	return 0;

	

}

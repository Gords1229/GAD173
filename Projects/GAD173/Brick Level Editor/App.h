#pragma once
#include "SFML/Graphics.hpp"

class App
{
private:
	sf::Event			event;
	sf::View			view;
	sf::RenderWindow	window;
	sf::Font			font;

	//other data members here

	sf::CircleShape		circle;
	float				radius;
	
	sf::Clock			clock;
	float				deltaTime;
	float				time;

	float				xSpeed;
	float				ySpeed;

	sf::Text			frameTime;
	sf::Text			totalTime;

public:
	App(const char* title, int screenWidth, int screenHeight, int screenBpp);

	~App();

	bool Init();
	void HandleEvents();
	void Draw();
	void Update();
	void Run();

private:
	//private methods
};

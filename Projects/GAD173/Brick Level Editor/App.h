#pragma once
#include "SFML/Graphics.hpp"

const int brickCol = 6;
const int brickRow = 4;

class App
{
private:
	sf::Event			event;
	sf::View			view;
	sf::RenderWindow	window;
	sf::Font			font;

	//other data members here

	//ball variables
	sf::CircleShape		circle;
	float				radius;
	//sf::Vector2f		initialLaunchVector;
	float				initialLanchAngle;
	float				PI;
	float				speed;

	float				xSpeed;
	float				ySpeed;

	//paddle variables
	sf::RectangleShape	paddle;
	float				paddleWidth;
	float				paddleHeight;
	float				paddleSpeed;
	float				yPaddlePosition;

	//brick variables
	sf::RectangleShape	bricks[brickCol][brickRow];
	float				brickWidth;
	float				brickHeight;
	float				brickGap;
	float				xBrickPad;
	float				yBrickPad;
	bool				isCollidable[brickCol][brickRow];

	//collision variables
	float				collisionAngle;
	float				colliderSideAngle;
	sf::Vector2f		ballCentre;
	sf::Vector2f		colliderCenter;
	
	//clock variables
	sf::Clock			clock;
	float				deltaTime;
	float				clockTime;	

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

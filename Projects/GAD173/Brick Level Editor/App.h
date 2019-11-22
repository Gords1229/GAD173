#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

//using namespace std;

//const int brickCol = 6;
//const int brickRow = 4;

class App
{
private:
	sf::Event			event;
	sf::View			view;
	sf::RenderWindow	window;
	sf::Font			font;

	//other data members here
	bool				isplaying;

	//button variables
	float				buttonWidth;
	float				buttonHeight;
	float				buttonGap;
	sf::Color			buttonTextColour;

<<<<<<< HEAD
=======
	sf::RectangleShape	startButton;
	sf::Vector2f		startButtonPosition;
	sf::Text			startButtonText;

	sf::RectangleShape	saveButton;
	sf::Vector2f		saveButtonPosition;
	sf::Text			saveButtonText;

	sf::RectangleShape	loadButton;
	sf::Vector2f		loadButtonPosition;
	sf::Text			loadButtonText;

	sf::RectangleShape	randomButton;
	sf::Vector2f		randomButtonPosition;
	sf::Text			randomButtonText;

>>>>>>> parent of fc1ca0f... tidied up the button code
	//save data variables
	std::string			saveData;
	std::ofstream		writeSaveFile;
	std::ifstream		readSaveFile;
	
	//ball variables
	sf::CircleShape		ball;
	float				radius;
	float				initialLanchAngle;
	sf::Vector2f		initialBallPosition;
	float				PI;
	float				ballSpeed;

	float				xSpeed;
	float				ySpeed;

	//paddle variables
	sf::RectangleShape	paddle;
	float				paddleWidth;
	float				paddleHeight;
	float				paddleSpeed;
	sf::Vector2f		initialPaddlePosition;
	
	//brick variables
	int					brickColumns;
	int					brickRows;
	sf::RectangleShape**bricks;
	float				brickWidth;
	float				brickHeight;
	float				brickGap;
	float				xBrickPad;
	float				yBrickPad;
	bool**				isCollidable;

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

	////sound variables
	//sf::Sound			audioSource;
	//sf::SoundBuffer		brickDestroyed;


public:
	App(const char* title, int screenWidth, int screenHeight, int screenBpp);

	~App();

	bool Init();
	void HandleEvents();
	void Draw();
	void Update();
	void Run();

	void CreateBrickArray();

private:
	//private methods
};

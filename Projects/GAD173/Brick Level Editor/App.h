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
	bool				isGame;
	bool				isEditor;
	bool				createdBricks;
	int					ballCount;
	int					maxBallCount;
	sf::CircleShape		ballCountIcon;
	/*sf::Color			ballCountIconColour;*/
	float				ballCountIconRadius;
	sf::Text			ballCountText;

	//menu button variables
	int					numberOfSaves;
	sf::RectangleShape**menuButtons;
	sf::Text**			menuButtonText;
	float				menuButtonWidth;
	float				menuButtonHeight;
	float				menuButtonGap;
	sf::Color			menuButtonTextColour;
	sf::RectangleShape	instructionButton;
	sf::Text			instructionButtonText;
	bool				instructionsOn;

	//button variables
	int					numberOfButtons;
	sf::RectangleShape* button;
	sf::Text*			buttonText;
	std::string*		buttonNames;
	float				buttonWidth;
	float				buttonHeight;
	float				buttonGap;
	sf::Color			buttonTextColour;

	//notes variables
	sf::Text			editorInstructionText;
	sf::Text			gameInstructionText;
	sf::Text			gameTitleText;

	//save data variables
	std::ofstream		writeSaveFile;
	std::ifstream		readSaveFile;
	std::string			targetFile;
	std::string			level1Save;
	std::string			level2Save;
	std::string			level3Save;
	std::string			custom1Save;
	std::string			custom2Save;
	std::string			custom3Save;
	
	//ball variables
	sf::CircleShape		ball;
	float				ballRadius;
	float				initialLanchAngle;
	sf::Vector2f		initialBallPosition;
	float				PI;
	float				ballSpeed;
	//float				ballSpeedModifier;
	float				xBallSpeed;
	float				yBallSpeed;

	//paddle variables
	sf::RectangleShape	paddle;
	float				paddleWidth;
	float				paddleHeight;
	float				paddleSpeed;
	float				defaultPaddleSpeed;
	sf::Vector2f		initialPaddlePosition;
	bool				paddleParalyzed;
	
	//brick variables
	int					brickColumns;
	int					minBrickColumns;
	int					maxBrickColumns;
	int					brickRows;
	int					minBrickRows;
	int					maxBrickRows;
	sf::RectangleShape**bricks;
	float				brickWidth;
	float				defaultBrickWidth;
	float				brickHeight;
	float				defaultBrickHeight;
	float				brickGap;
	float				xBrickPad;
	float				yBrickPad;
	int					defaultBrickHealth; //the most health that can be set in editor
	int					maxBrickHealth; //the most health each brick has in game
	int**				brickHealth;
	sf::Color			brickFullHealthColour;
	sf::Color			brickDamagedHealthColour;
	sf::Color			brickCriticalHealthColour;
	sf::Color			solidBrickColour;
	bool**				isCollidable;
	int**				brickID;
	int					colouredBricksRemaining;
	
	//power up and hazard brick variables
	int					numberOfBrickTypes;
	float				powerUpFallSpeed;

	sf::CircleShape**	paddleUpgrade;	
	sf::Color			paddleUpgradeColour;
	int					paddleUpgradeHitCounter;
	int					paddleUpgradeMaxHits;

	sf::CircleShape**	paralyzer;
	sf::Color			paralyzerColour;
	float**				paralyzerLaunchAngle;
	int					paralyzerStunTime;
	int					paddleStunTimer;
	bool**				paralyzerActive;
	
	// all power up and hazard IDs must be odd numbers.
	int					paddleUpgradeID;
	int					solidBrickID;
	float				paralyzerID;

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

	//sound variables
	sf::Sound				soundSourceBrickDestroy;
	sf::Sound				soundSourcePaddleHit;
	sf::Sound				soundSourceParalyzed;
	sf::Sound				soundSourceBigger;
	sf::Sound				soundSourceButton;
	sf::Sound				soundSourceSolidHit;
	sf::SoundBuffer			brickDestroyed;
	sf::SoundBuffer			paddleHit;
	sf::SoundBuffer			paralyzedSound;
	sf::SoundBuffer			biggerSound;
	sf::SoundBuffer			smallerSound;
	sf::SoundBuffer			buttonSound;
	sf::SoundBuffer			backButtonSound;
	sf::SoundBuffer			solidHitSound;
	sf::SoundBuffer			lostBallSound;

	sf::Music				music;
	


public:
	App(const char* title, int screenWidth, int screenHeight, int screenBpp);

	~App();

	bool Init();
	void HandleEvents();
	void Draw();
	void Update();
	void Run();

	void CreateBrickArray();
	void LoadLevel();
	void StartGame();

private:
	//private methods
};

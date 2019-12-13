#include <fstream>
#include "App.h"

//contructor
App::App(const char* title, int screenWidth, int screenHeight, int screenBpp) {
	window.create(
		sf::VideoMode(screenWidth, screenHeight, screenBpp), title);

	window.setFramerateLimit(0);
	view = window.getDefaultView();
}

//destructor
App::~App() {
	//release memory

	if (createdBricks) {
		for (int i = 0; i < brickColumns; ++i) {
			delete[] bricks[i];
			delete[] isCollidable[i];
			delete[] brickHealth[i];
			delete[] brickID[i];
			delete[] paddleUpgrade[i];
			delete[] paralyzer[i];
			delete[] paralyzerActive[i];
			delete[] paralyzerLaunchAngle[i];
		}

		delete[] bricks;
		delete[] isCollidable;
		delete[] brickHealth;
		delete[] brickID;
		delete[] paddleUpgrade;
		delete[] paralyzer;
		delete[] paralyzerActive;
		delete[] paralyzerLaunchAngle;
	}

	for (int i = 0; i < 2; i++) {
		delete[] menuButtons[i];
		delete[] menuButtonText[i];
	}

	delete[] menuButtons;
	delete[] menuButtonText;
}

bool App::Init() {
	srand(time(NULL));

	music.stop();
	if (!music.openFromFile("ThemeMusic_mui_mus_2.00.wav")) {
		std::cout << "ThemeMusic not loaded" << std::endl;
	}
	music.setVolume(80.0f);
	music.setLoop(true);
	music.play();
	
	//initialise App data members
	font.loadFromFile("arial.ttf");
	isGame = false;
	isEditor = false;
	createdBricks = false;

	level1Save = "level1SaveData.txt";
	level2Save = "level2SaveData.txt";
	level3Save = "level3SaveData.txt";
	custom1Save = "custom1SaveData.txt";
	custom2Save = "custom2SaveData.txt";
	custom3Save = "custom3SaveData.txt";

	//initialise ball count
	maxBallCount = 3;
	ballCountIconRadius = 30.0f;
	ballCountIcon.setRadius(ballCountIconRadius);
	ballCountIcon.setFillColor(sf::Color::Green);
	ballCountIcon.setOrigin(sf::Vector2f(ballCountIcon.getRadius(), ballCountIcon.getRadius()));
	ballCountIcon.setPosition(window.getSize().x - ballCountIconRadius, ballCountIconRadius);
	ballCountText.setFont(font);
	ballCountText.setCharacterSize(1.6f * ballCountIconRadius);
	ballCountText.setFillColor(sf::Color::Black);
	//ballCountText.setOrigin(0.5f * ballCountText.getGlobalBounds().width, 0.5f * ballCountText.getGlobalBounds().height);
	ballCountText.setPosition(window.getSize().x - 1.5f * ballCountIconRadius, 0);

	//initialise sound
	if (!brickDestroyed.loadFromFile("AlanOw!.wav")) {
		std::cout << "AlanOw! sound not loaded"<<std::endl;
	}
		
	if (!paddleHit.loadFromFile("paddleHit.wav")) {
		std::cout << "ArrowShot_itm_sfx_v1.00 sound not loaded"<<std::endl;
	}

	if (!paralyzedSound.loadFromFile("paralyzedSFX.wav")) {
		std::cout << "ArrowShot_itm_sfx_v1.00 sound not loaded" << std::endl;
	}

	if (!buttonSound.loadFromFile("buttonSFX.wav")) {
		std::cout << "ArrowShot_itm_sfx_v1.00 sound not loaded" << std::endl;
	}

	if (!solidHitSound.loadFromFile("solidHitSFX.wav")) {
		std::cout << "ArrowShot_itm_sfx_v1.00 sound not loaded" << std::endl;
	}

	if (!smallerSound.loadFromFile("smallSFX.wav")) {
		std::cout << "ArrowShot_itm_sfx_v1.00 sound not loaded" << std::endl;
	}

	if (!biggerSound.loadFromFile("bigSFX.wav")) {
		std::cout << "ArrowShot_itm_sfx_v1.00 sound not loaded" << std::endl;
	}

	if (!backButtonSound.loadFromFile("backButton.wav")) {
		std::cout << "backButton sound not loaded" << std::endl;
	}
	
	if (!lostBallSound.loadFromFile("lostBallSFX.wav")) {
		std::cout << "lostBallSFX sound not loaded" << std::endl;
	}

	//initialise timers
	frameTime.setFont(font);
	frameTime.setCharacterSize(24);
	totalTime.setFont(font);
	totalTime.setCharacterSize(24);
	totalTime.setPosition(0, 34);
	
	//initialise paddle
	paddleWidth = 150.0f;
	paddleHeight = 20.0f;
	paddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	
	paddle.setOrigin(sf::Vector2f(0.5 * paddle.getSize().x, 0.5 * paddle.getSize().y));
	
	initialPaddlePosition = sf::Vector2f(window.getSize().x * 0.5f, window.getSize().y - 50.0f);
	paddle.setPosition(initialPaddlePosition);

	defaultPaddleSpeed = 500.0f;
	paddleParalyzed = false;

	//initialise ball
	
	PI = 2 * asin(1);

	ballRadius = 10.0f;
	ball.setRadius(ballRadius);

	ball.setOrigin(sf::Vector2f(ball.getRadius(), ball.getRadius()));

	initialBallPosition = sf::Vector2f(window.getSize().x * 0.5f, paddle.getPosition().y - 0.5f * paddle.getSize().y - ballRadius);
	ball.setPosition(initialBallPosition);

	// initialise power ups and hazards
	numberOfBrickTypes = 4;
	powerUpFallSpeed = 200.0f;
	// all power up and hazard IDs must be odd numbers.
	paddleUpgradeID = 3;
	paddleUpgradeColour = sf::Color::Blue;
	paddleUpgradeMaxHits = 3;
	paddleUpgradeHitCounter = 0;

	solidBrickID = 5;
	solidBrickColour = sf::Color::White;

	paralyzerID = 7;
	paralyzerColour = sf::Color::Cyan;
	paralyzerStunTime = 1000.0f;
	paddleStunTimer = paralyzerStunTime;

	//initialise bricks
	//set bricks array size
	brickColumns = 8;
	minBrickColumns = 6;
	maxBrickColumns = 40;
	brickRows = 5;
	minBrickRows = 4;
	maxBrickRows = 12;
	colouredBricksRemaining = brickColumns * brickRows;
	
	defaultBrickWidth = 200.0f;
	defaultBrickHeight = 50.0f;
	brickGap = 3.0f;
	yBrickPad = 100.0f;

	defaultBrickHealth = 3;
	maxBrickHealth = defaultBrickHealth;
	brickFullHealthColour = sf::Color::Green;
	brickDamagedHealthColour = sf::Color::Yellow;
	brickCriticalHealthColour = sf::Color::Red;
	
	//CreateBrickArray();

	//intialise buttons
	numberOfButtons = 11;
	buttonNames = new std::string[numberOfButtons]{
		"START", "SAVE", "RESET", "+ COL", "- COL", "+ ROW", "- ROW", "+ BRICK HEALTH", "- BRICK HEALTH", "RANDOMIZE", "BACK"
	};

	//button dimensions
	if (150.0f * (1.05f * numberOfButtons + 0.05f) > window.getSize().x) {
		buttonWidth = window.getSize().x / (numberOfButtons * 1.05f + 0.05f);
	}
	else {
		buttonWidth = 150.0f;
	}
		
	buttonHeight = buttonWidth / 3.0f;
	buttonGap = 0.05f * buttonWidth;
	buttonTextColour = sf::Color::Black;

	/*std::cout << "button width " << buttonWidth << std::endl;
	std::cout << "button height " << buttonHeight << std::endl;
	std::cout << "button gap " << buttonGap << std::endl;*/

	//placing the buttons
	button = new sf::RectangleShape[numberOfButtons];
	buttonText = new sf::Text[numberOfButtons];

	for (int i = 0; i < numberOfButtons; i++) {
		button[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
		button[i].setOrigin(sf::Vector2f(0.5f * button[i].getSize().x, 0.5f * button[i].getSize().y));		
		button[i].setPosition(window.getSize().x - (0.5f * buttonWidth + buttonGap) - (numberOfButtons - 1 - i) * (buttonWidth + buttonGap), 0.5f * buttonHeight + 1);

		//setting button text
		buttonText[i].setString(buttonNames[i]);
		buttonText[i].setFont(font);
		buttonText[i].setCharacterSize(0.6f * buttonHeight);
		buttonText[i].setOrigin(sf::Vector2f(0.5f * buttonText[i].getGlobalBounds().width, /*0.7f **/ buttonText[i].getGlobalBounds().height));
		
		/*std::cout << "button[" << i << "] position : text[" << i << "] position -  " << 
			button[i].getPosition().x << " , " << button[i].getPosition().y << " : " << 
			buttonText[i].getPosition().x << " , " << buttonText[i].getPosition().y << std::endl;*/

		//sizing text to fit in the button
		if (buttonText[i].getGlobalBounds().width > 0.9f * buttonWidth) {
			buttonText[i].setScale((0.9f * buttonWidth) / buttonText[i].getGlobalBounds().width, (0.9f * buttonWidth) / buttonText[i].getGlobalBounds().width);
		}
			   		
		buttonText[i].setPosition(button[i].getPosition());
		buttonText[i].setFillColor(buttonTextColour);
	}

	//initialise editor notes

	editorInstructionText.setString("Left mouse click on a brick to toggle it on or off. \n\n"
		"Right mouse click on a brick to cycle it through the bricks type. \n\n"
		"Use the buttons on the top to change the size of the grid and the max health of the bricks.");
	editorInstructionText.setFont(font);
	editorInstructionText.setCharacterSize(38);
	editorInstructionText.setPosition(100, window.getSize().y - 350);
		
	gameTitleText.setString("BRICK CLONE GAME");
	gameTitleText.setFont(font);
	gameTitleText.setCharacterSize(100);
	gameTitleText.setFillColor(sf::Color::Magenta);
	gameTitleText.setOrigin(0.5f * gameTitleText.getGlobalBounds().width, 0.0f);
	gameTitleText.setPosition(0.5f * window.getSize().x, 50.0f);

	gameInstructionText.setString("Move the paddle left and right with the 'A' and 'D' keys respectively. \n\n"
		"Stop the ball from hitting the ground by moving the paddle under the ball. \n\n"

		"If the paddle is moving when the ball bounces on it the sideways motion of the ball will change. \n"
		"If the paddle and ball are moving in the oppiste direction the ball slows down and\n"
		"if they are moving in the same direction the ball speeds up. \n\n"

		"Destroy all the coloured bricks before letting the ball hit the ground 3 times to finish a level. \n"
		"The coloured bricks are destroyed when they have been hit enough.\n"
		"The brick's colour will go yellow to red when taken damage. \n\n"

		"Dark Blue Objects are Paddle Size Upgrades that will increase the size of the paddle for 3 hits.\n"
		"They fall when their containing brick is destroyed. Move the paddle to the upgrade to use. \n\n"

		"The white bricks can not be destroyed and some white bricks will shoot paralyzers at you if they get hit.\n"
		"The paralyzers are small light blue triangles, avoid them or your paddle will be frozen for a short time.\n\n"

		"There are 3 levels to try or create your own using the editor. \n\n"
		"Have Fun!!!");
	gameInstructionText.setFont(font);
	gameInstructionText.setCharacterSize(38);
	gameInstructionText.setPosition(100, 100);

	//initialise menu variables
	instructionsOn = false;
	numberOfSaves = 3;
	menuButtonGap = 200.0f;
	menuButtonWidth = (window.getSize().x - (numberOfSaves + 3) * menuButtonGap) / numberOfSaves;
	menuButtonHeight = menuButtonWidth / 3;
	menuButtonTextColour = sf::Color::Black;

	// dynamically allocate memory for an array of pointers to various menu arrays
	menuButtons = new sf::RectangleShape * [2];
	menuButtonText = new sf::Text * [2];
	
	// dynamically allocate memory for arrays of above arrays
	for (int i = 0; i < 2; ++i) {
		menuButtons[i] = new sf::RectangleShape[numberOfSaves];
		menuButtonText[i] = new sf::Text[numberOfSaves];
	}

	// initialise and output the 2D array of brick and their other attributes
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < numberOfSaves; j++) {
			menuButtons[i][j].setSize(sf::Vector2f(menuButtonWidth, menuButtonHeight));
			menuButtons[i][j].setOrigin(sf::Vector2f(0.5 * menuButtons[i][j].getSize().x, 0.5 * menuButtons[i][j].getSize().y));
			menuButtons[i][j].setPosition(sf::Vector2f(2 * menuButtonGap + 0.5f * menuButtonWidth + j * (menuButtonWidth + menuButtonGap), window.getSize().y - 3 * menuButtonGap - 1.5f * menuButtonHeight + i * (menuButtonHeight + menuButtonGap)));
			
			if (i == 0) {
				menuButtonText[i][j].setString("LEVEL " + std::to_string(j + 1));
			}
			else if (i == 1) {
				menuButtonText[i][j].setString("CUSTOM " + std::to_string(j + 1));
			}

			menuButtonText[i][j].setFont(font);
			menuButtonText[i][j].setCharacterSize(0.6f * menuButtonHeight);
			menuButtonText[i][j].setOrigin(sf::Vector2f(0.5 * menuButtonText[i][j].getGlobalBounds().width, menuButtonText[i][j].getGlobalBounds().height));

			//std::cout << menuButtonText[i][j].getGlobalBounds().height << " " << menuButtonText[i][j].getOrigin().y << std::endl;

			//sizing text to fit in the button
			if (menuButtonText[i][j].getGlobalBounds().width > 0.9f * menuButtonWidth) {
				menuButtonText[i][j].setScale((0.9f * menuButtonWidth) / menuButtonText[i][j].getGlobalBounds().width, (0.9f * menuButtonWidth) / menuButtonText[i][j].getGlobalBounds().width);
			}

			menuButtonText[i][j].setPosition(menuButtons[i][j].getPosition());
			menuButtonText[i][j].setFillColor(menuButtonTextColour);
		}
	}

	instructionButton.setSize(sf::Vector2f(menuButtonWidth, menuButtonHeight));
	instructionButton.setOrigin(0.5 * instructionButton.getSize().x, 0.5 * instructionButton.getSize().y);
	instructionButton.setPosition(menuButtons[1][1].getPosition().x, window.getSize().y - menuButtonGap);

	instructionButtonText.setString("INSTRUCTIONS");
	instructionButtonText.setFont(font);
	instructionButtonText.setCharacterSize(0.6f * menuButtonHeight);
	instructionButtonText.setOrigin(0.5f * instructionButton.getGlobalBounds().width + 60, 0.5f * instructionButton.getGlobalBounds().height);

	if (instructionButtonText.getGlobalBounds().width > 0.9f * menuButtonWidth) {
		instructionButtonText.setScale((0.9f * menuButtonWidth) / instructionButtonText.getGlobalBounds().width, (0.9f * menuButtonWidth) / instructionButtonText.getGlobalBounds().width);
	}

	instructionButtonText.setPosition(instructionButton.getPosition());
	instructionButtonText.setFillColor(menuButtonTextColour);

	return true;
}

void App::Update() {
	//update
	deltaTime = clock.restart().asSeconds();
	frameTime.setString(std::to_string(deltaTime));
	//std::cout << deltaTime << std::endl;

	if (isGame) {
		clockTime += deltaTime;
		totalTime.setString(std::to_string(clockTime));
		//std::cout << time << std::endl;

		//user Input
		if (paddleParalyzed == false) {
			paddle.setFillColor(sf::Color::White);
			paddleStunTimer = paralyzerStunTime;
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				// A key is pressed: move our paddle left
				if (paddle.getPosition().x >= 0.5f * paddle.getSize().x) {
					paddleSpeed = -1 * defaultPaddleSpeed;
					paddle.move(paddleSpeed * deltaTime, 0.f);
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				// D key is pressed: move our paddle right
				if (paddle.getPosition().x <= window.getSize().x - 0.5f * paddle.getSize().x) {
					paddleSpeed = defaultPaddleSpeed;
					paddle.move(paddleSpeed * deltaTime, 0.f);
				}
			}
			else {
				paddleSpeed = 0.0f;
			}
		}
		else {
			paddle.setFillColor(sf::Color::Cyan);

			paddleStunTimer -= deltaTime;

			/*sound.setBuffer(paralyzedSound);
			sound.play();*/

			if (paddleStunTimer <= 0.0f) {
				paddleParalyzed = false;

				//sound.stop();
			}
		}

		//ball motion
		ball.move(xBallSpeed * deltaTime, yBallSpeed * deltaTime);

		//game logic

		//left collision border detection
		if (ball.getPosition().x < ballRadius) {
			ball.setPosition(ballRadius, ball.getPosition().y);
			xBallSpeed = -xBallSpeed;

			soundSourceSolidHit.setBuffer(solidHitSound);
			soundSourceSolidHit.play();
		}

		//right collision border detection
		if (ball.getPosition().x > window.getSize().x - ballRadius) {
			ball.setPosition(window.getSize().x - ballRadius, ball.getPosition().y);
			xBallSpeed = -xBallSpeed;

			soundSourceSolidHit.setBuffer(solidHitSound);
			soundSourceSolidHit.play();
		}

		//top collision border detection
		if (ball.getPosition().y < ballRadius) {
			ball.setPosition(ball.getPosition().x, ballRadius);
			yBallSpeed = -yBallSpeed;

			soundSourceSolidHit.setBuffer(solidHitSound);
			soundSourceSolidHit.play();
		}

		//bottom collision border detection
		if (ball.getPosition().y > window.getSize().y - ballRadius) {
			ballCount--;

			soundSourceSolidHit.setBuffer(lostBallSound);
			soundSourceSolidHit.play();

			if (ballCount == maxBallCount) {
				ballCountIcon.setFillColor(brickFullHealthColour);
				ball.setPosition(initialBallPosition);
				paddle.setPosition(initialPaddlePosition);
				paddleParalyzed = false;
				initialLanchAngle = (rand() % 120 + 30) * PI / 180;
				std::cout << "intialLaunchAngle = " << initialLanchAngle * 180 / PI << std::endl;
				ballSpeed = 500.0f;
				xBallSpeed = ballSpeed * cos(initialLanchAngle);
				yBallSpeed = -ballSpeed * sin(initialLanchAngle);
			}
			else if (ballCount == 1) {
				ballCountIcon.setFillColor(brickCriticalHealthColour);
				ball.setPosition(initialBallPosition);
				paddle.setPosition(initialPaddlePosition);
				paddleParalyzed = false;
				initialLanchAngle = (rand() % 120 + 30) * PI / 180;
				std::cout << "intialLaunchAngle = " << initialLanchAngle * 180 / PI << std::endl;
				ballSpeed = 500.0f;
				xBallSpeed = ballSpeed * cos(initialLanchAngle);
				yBallSpeed = -ballSpeed * sin(initialLanchAngle);
			}
			else if (ballCount <= 0) {
				isGame = false;

				music.stop();
				if (!music.openFromFile("ThemeMusic_mui_mus_2.00.wav")) {
					std::cout << "ThemeMusic not loaded" << std::endl;
				}
				music.setLoop(true);
				music.play();

				ballSpeed = 0.0f;
				ball.setPosition(initialBallPosition);
				paddle.setPosition(initialPaddlePosition);
				paddleParalyzed = false;
				clockTime = 0.0f;

				if (isEditor == true) {
					readSaveFile.open("tempSaveData.txt");
					if (readSaveFile.is_open()) {

						readSaveFile >> brickColumns;
						readSaveFile >> brickRows;
						readSaveFile >> maxBrickHealth;

						CreateBrickArray();

						while (!readSaveFile.eof()) {
							for (int i = 0; i < brickColumns; i++) {
								for (int j = 0; j < brickRows; j++) {
									readSaveFile >> brickID[i][j];

									isCollidable[i][j] = brickID[i][j] % 2;
									brickHealth[i][j] = maxBrickHealth;
								}
							}
						}

						readSaveFile.close();
					}
					else {
						std::cout << "unable to open file" << std::endl;
					}
				}			
			}
			else {
				ballCountIcon.setFillColor(brickDamagedHealthColour);
				ball.setPosition(initialBallPosition);
				paddle.setPosition(initialPaddlePosition);
				paddleParalyzed = false;
				initialLanchAngle = (rand() % 120 + 30) * PI / 180;
				std::cout << "intialLaunchAngle = " << initialLanchAngle * 180 / PI << std::endl;
				ballSpeed = 500.0f;
				xBallSpeed = ballSpeed * cos(initialLanchAngle);
				yBallSpeed = -ballSpeed * sin(initialLanchAngle);
			}

			ballCountText.setString(std::to_string(ballCount));
		}

		//brick collision
		for (int i = 0; i < brickColumns; i++) {
			for (int j = 0; j < brickRows; j++) {

				if (ball.getGlobalBounds().intersects(bricks[i][j].getGlobalBounds()) && isCollidable[i][j] == true) {

					if (brickID[i][j] != solidBrickID && brickID[i][j] != paralyzerID) {
						brickHealth[i][j]--;

						if (brickHealth[i][j] == 0) {
							colouredBricksRemaining--;

							soundSourceBrickDestroy.setBuffer(brickDestroyed);
							soundSourceBrickDestroy.setPitch(1.2f);
							soundSourceBrickDestroy.play();
						}
						else {
							soundSourceBrickDestroy.setBuffer(brickDestroyed);
							soundSourceBrickDestroy.setPitch(1.0f);
							soundSourceBrickDestroy.play();
						}						
					}
					else {
						soundSourceSolidHit.setBuffer(solidHitSound);
						soundSourceSolidHit.play();
					}

					if (brickID[i][j] == paralyzerID && paralyzerActive[i][j] == false && paddleParalyzed == false) {
						paralyzerActive[i][j] = true;
						paralyzerLaunchAngle[i][j] = atan2(paralyzer[i][j].getPosition().y - paddle.getPosition().y, paralyzer[i][j].getPosition().x - (paddle.getPosition().x + paddleSpeed));
						//paralyzer[i][j].move((powerUpFallSpeed * cos(paralyzerLaunchAngle)) * deltaTime, (abs(powerUpFallSpeed * sin(paralyzerLaunchAngle))) * deltaTime);
					}

					//determining the position of the collision
					colliderSideAngle = abs(atan2(ballRadius + 0.5f * bricks[i][j].getSize().y, ballRadius + 0.5f * bricks[i][j].getSize().x));
					collisionAngle = abs(atan2(ball.getPosition().y - bricks[i][j].getPosition().y, ball.getPosition().x - bricks[i][j].getPosition().x));

					if (collisionAngle < PI - colliderSideAngle && collisionAngle > colliderSideAngle) {
						//check which side the ball hit the paddle (top or bottom) and adjust the ball position accordingly
						if (ball.getPosition().y > bricks[i][j].getPosition().y) {
							ball.setPosition(ball.getPosition().x, bricks[i][j].getPosition().y + 0.5f * bricks[i][j].getSize().y + ballRadius);
						}
						else {
							ball.setPosition(ball.getPosition().x, bricks[i][j].getPosition().y - 0.5f * bricks[i][j].getSize().y - ballRadius);
						}
						yBallSpeed = -yBallSpeed;
					}
					else {
						//check which side the ball hit the paddle (left or right) and adjust the ball position accordingly
						if (ball.getPosition().x > bricks[i][j].getPosition().x) {
							ball.setPosition(bricks[i][j].getPosition().x + 0.5f * bricks[i][j].getSize().x + ballRadius, ball.getPosition().y);
						}
						else {
							ball.setPosition(bricks[i][j].getPosition().x - 0.5f * bricks[i][j].getSize().x - ballRadius, ball.getPosition().y);
						}

						xBallSpeed = -xBallSpeed;
					}
				}

				//power up and hazard movement
				if (brickHealth[i][j] == 0) {
					if (brickID[i][j] == paddleUpgradeID) {
						paddleUpgrade[i][j].move(0.0f, powerUpFallSpeed * deltaTime);
					}
				}

				if (paralyzerActive[i][j] == true) {
					//paralyzerLaunchAngle = abs(atan2(paralyzer[i][j].getPosition().y - paddle.getPosition().y, paralyzer[i][j].getPosition().x - (paddle.getPosition().x + paddleSpeed)));
					paralyzer[i][j].move(-(powerUpFallSpeed* cos(paralyzerLaunchAngle[i][j]))* deltaTime, (abs(powerUpFallSpeed * sin(paralyzerLaunchAngle[i][j]))) * deltaTime);
					paralyzer[i][j].rotate(10.0f);

					if (paralyzer[i][j].getPosition().y > window.getSize().y + 2 * paralyzer[i][j].getRadius()) {
						paralyzerActive[i][j] = false;
					}
				}
				else {
					paralyzer[i][j].setPosition(bricks[i][j].getPosition());
					paralyzer[i][j].setRotation(0.0f);
				}
			}
		}

		//paddle collision
		if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {

			soundSourcePaddleHit.setBuffer(paddleHit);
			soundSourcePaddleHit.play();

			if (paddleUpgradeHitCounter > 0) {
				std::cout << paddleUpgradeHitCounter << "counter" << std::endl;
				paddleUpgradeHitCounter--;

				if (paddleUpgradeHitCounter == 0) {
					soundSourceBigger.setBuffer(smallerSound);
					soundSourceBigger.play();

					yBallSpeed = -yBallSpeed;

					paddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
					paddle.setOrigin(sf::Vector2f(0.5 * paddle.getSize().x, 0.5 * paddle.getSize().y));
				}
			}
			/*else {
				
			}*/

			//determining the position of the collision
			colliderSideAngle = abs(atan2(ballRadius + 0.5f * paddle.getSize().y, ballRadius + 0.5f * paddle.getSize().x));
			collisionAngle = abs(atan2(ball.getPosition().y - paddle.getPosition().y, ball.getPosition().x - paddle.getPosition().x));

			if (collisionAngle < PI - colliderSideAngle && collisionAngle > colliderSideAngle) {
				//check which side the ball hit the paddle (bottom or top) and adjust the ball position accordingly and speed accordingly
				if (ball.getPosition().y > paddle.getPosition().y) {
					ball.setPosition(ball.getPosition().x, paddle.getPosition().y + 0.5f * paddle.getSize().y + ballRadius);
				}
				else {
					ball.setPosition(ball.getPosition().x, paddle.getPosition().y - 0.5f * paddle.getSize().y - ballRadius);
				}
				yBallSpeed = -yBallSpeed;
				//quick and simple method of effecting ball motion
				if (xBallSpeed == 0) {
					xBallSpeed += 0.1f * paddleSpeed;
				}				
				else if (xBallSpeed / abs(xBallSpeed) == paddleSpeed / abs(paddleSpeed) && abs(xBallSpeed) <= 0.8f * defaultPaddleSpeed) {
					xBallSpeed += (xBallSpeed / abs(xBallSpeed)) * 0.5f * (abs(0.8f * paddleSpeed) - abs(xBallSpeed));
				}
				else if (xBallSpeed / abs(xBallSpeed) == -(paddleSpeed / abs(paddleSpeed))) {
					//xBallSpeed += -(xBallSpeed / abs(xBallSpeed)) * 0.5f * (abs(paddleSpeed) - abs(xBallSpeed));
					xBallSpeed *= 0.5f;
				}

				// For the ball x directional speed modifier, if the paddle speed is not 0,
				// take the difference of absolutes of the paddle speed and ball horizontal speed as a percentage of the paddle speed.
				// multiply this by the sign of the ball movement to get the multiplier.
				// The sign of this multiplier will now be negative if ball and paddle are moving in the opposite direction, positive otherwise.
				// Then we add that percent of the ball horizontal speed to the current ball horizontal speed.
				//if (paddleSpeed != 0) {
				//	ballSpeedModifier = (abs((paddleSpeed - xBallSpeed) / paddleSpeed)) - 1 /** (xBallSpeed / abs(xBallSpeed))*/;
				//	std::cout << "ball speed is " << xBallSpeed << std::endl;
				//	std::cout << "paddle speed is " << paddleSpeed << std::endl;
				//	//std::cout << "abs diff is " << abs(paddleSpeed - xBallSpeed) << std::endl;
				//	std::cout << "percent diff is " << (abs((paddleSpeed - xBallSpeed) / paddleSpeed)) << std::endl;
				//	std::cout << "ball direction is " << xBallSpeed / abs(xBallSpeed) << std::endl;
				//	std::cout << "multiplier is " << ((abs(paddleSpeed - xBallSpeed)) / paddleSpeed) - 1 << std::endl;
				//	
				//	xBallSpeed += ballSpeedModifier * xBallSpeed;
				//	std::cout << "ball speed is " << xBallSpeed << std::endl;
				//}

			}
			else {
				//check which side the ball hit the paddle (right or left) and adjust the ball position accordingly
				if (ball.getPosition().x > paddle.getPosition().x) {
					ball.setPosition(paddle.getPosition().x + 0.5f * paddle.getSize().x + paddleSpeed * deltaTime + ballRadius, ball.getPosition().y);
				}
				else {
					ball.setPosition(paddle.getPosition().x - 0.5f * paddle.getSize().x - paddleSpeed * deltaTime - ballRadius, ball.getPosition().y);
				}
				xBallSpeed += paddleSpeed;
			}
		}

		//paddle upgrade and hazard collision
		for (int i = 0; i < brickColumns; i++) {
			for (int j = 0; j < brickRows; j++) {
				if (paddleUpgrade[i][j].getGlobalBounds().intersects(paddle.getGlobalBounds())) {
					//paddle.setScale(2.0f, 1.0f);
					soundSourceBigger.setBuffer(biggerSound);
					soundSourceBigger.play();

					paddleUpgrade[i][j].setPosition(-100, -100);
					paddle.setSize(sf::Vector2f(2 * paddleWidth, paddleHeight));
					paddle.setOrigin(sf::Vector2f(0.5 * paddle.getSize().x, 0.5 * paddle.getSize().y));
					paddleUpgradeHitCounter += paddleUpgradeMaxHits;
					brickID[i][j] = 0;

					std::cout << "paddle upgrade collected" << std::endl;
				}

				if (paralyzer[i][j].getGlobalBounds().intersects(paddle.getGlobalBounds())) {
					paddleParalyzed = true;
					paralyzerActive[i][j] = false;

					soundSourceParalyzed.setBuffer(paralyzedSound);
					soundSourceParalyzed.play();

					std::cout << "paddle is paralyzed " << paddleParalyzed << std::endl;
				}
			}
		}
	}

	if (colouredBricksRemaining <= 0) {
		colouredBricksRemaining = 1; //line to release this code from looping
		isGame = false;

		music.stop();
		if (!music.openFromFile("ThemeMusic_mui_mus_2.00.wav")) {
			std::cout << "ThemeMusic not loaded" << std::endl;
		}
		music.setLoop(true);
		music.play();

		ballSpeed = 0.0f;
		ball.setPosition(initialBallPosition);
		paddle.setPosition(initialPaddlePosition);
		paddleParalyzed = false;
		clockTime = 0.0f;		

		if (isEditor == true) {
			std::cout << "winner to editor" << std::endl;
			
			readSaveFile.open("tempSaveData.txt");
			if (readSaveFile.is_open()) {

				readSaveFile >> brickColumns;
				readSaveFile >> brickRows;
				readSaveFile >> maxBrickHealth;

				CreateBrickArray();

				while (!readSaveFile.eof()) {
					for (int i = 0; i < brickColumns; i++) {
						for (int j = 0; j < brickRows; j++) {
							readSaveFile >> brickID[i][j];

							isCollidable[i][j] = brickID[i][j] % 2;
							brickHealth[i][j] = maxBrickHealth;
						}
					}
				}

				readSaveFile.close();
			}
			else {
				std::cout << "unable to open file" << std::endl;
			}
		}		
	}

	if (isGame == true || isEditor == true) {
		//brick colour determination
		for (int i = 0; i < brickColumns; i++) {
			for (int j = 0; j < brickRows; j++) {
				if (brickID[i][j] != solidBrickID && brickID[i][j] != paralyzerID) {
					if (brickHealth[i][j] == defaultBrickHealth) {
						bricks[i][j].setFillColor(brickFullHealthColour);
					}
					else if (brickHealth[i][j] == 1) {
						bricks[i][j].setFillColor(brickCriticalHealthColour);
					}
					else if (brickHealth[i][j] == 0) {
						isCollidable[i][j] = false;
					}
					else {
						bricks[i][j].setFillColor(brickDamagedHealthColour);
					}
				}
				else {
					bricks[i][j].setFillColor(solidBrickColour);
				}
			}
		}
	}	
}

void App::Draw() {
	window.clear();
	window.setView(view);

	//draw	

	if (isGame == false) {		
		
		if (isEditor == false) {			

			if (instructionsOn == false) {
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < numberOfSaves; j++) {
						window.draw(menuButtons[i][j]);
						window.draw(menuButtonText[i][j]);						
					}
				}

				window.draw(instructionButton);
				window.draw(instructionButtonText);
				window.draw(gameTitleText);
			}
			else {
				window.draw(button[numberOfButtons - 1]);
				window.draw(buttonText[numberOfButtons - 1]);

				window.draw(gameInstructionText);
			}

		}
		else {
			for (int i = 0; i < numberOfButtons; i++) {
				window.draw(button[i]);
				window.draw(buttonText[i]);

				window.draw(editorInstructionText);
			}
		}			
	}
	else {
		window.draw(ball);
		window.draw(paddle);
		
		window.draw(totalTime);
		window.draw(ballCountIcon);
		window.draw(ballCountText);
	}

	if (isGame == true || isEditor == true) {
		
		//window.draw(frameTime);	
		
		for (int i = 0; i < brickColumns; i++) {
			for (int j = 0; j < brickRows; j++) {

				if (isCollidable[i][j] == true) {
					window.draw(bricks[i][j]);
				}

				if (brickID[i][j] == paddleUpgradeID) {
					window.draw(paddleUpgrade[i][j]);
				}

				if (brickID[i][j] == paralyzerID) {
					window.draw(paralyzer[i][j]);
				}
			}
		}
	}	

	window.display();
}

void App::HandleEvents() {
	if (event.type == sf::Event::Closed) {
		window.close();
	}

	if (isGame == true && sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		// R key is pressed: reset back to editor
		isGame = false;

		music.stop();
		if (!music.openFromFile("ThemeMusic_mui_mus_2.00.wav")) {
			std::cout << "ThemeMusic not loaded" << std::endl;
		}
		music.setLoop(true);
		music.play();

		ballSpeed = 0.0f;
		ball.setPosition(initialBallPosition);
		paddle.setPosition(initialPaddlePosition);
		clockTime = 0.0f;
	}

	if (event.type == sf::Event::MouseButtonPressed) {

		//get the local mouse position (relative to a window)
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			
			//std::cout << "Left Mouse Pressed" << std::endl;

			if (isEditor == true) {
				//mouse click position is in the brick bounds
				for (int i = 0; i < brickColumns; i++) {
					for (int j = 0; j < brickRows; j++) {
						if (bricks[i][j].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
							if (brickID[i][j] % 2 == 1) {
								brickID[i][j]--;
							}
							else {
								brickID[i][j]++;
							}

							isCollidable[i][j] = brickID[i][j] % 2;
						}
					}
				}

				if (isGame == false) {

					//mouse click position is in the start button
					if (button[0].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
						isGame = true;

						soundSourceButton.setBuffer(buttonSound);
						soundSourceButton.play();

						writeSaveFile.open("tempSaveData.txt");
						if (writeSaveFile.is_open()) {

							writeSaveFile << brickColumns << " " << brickRows << " " << maxBrickHealth << "\n";

							for (int i = 0; i < brickColumns; i++) {
								for (int j = 0; j < brickRows; j++) {
									writeSaveFile << brickID[i][j] << " ";
								}
							}

							writeSaveFile.close();
						}
						else {
							std::cout << "unable to open file" << std::endl;
						}
						StartGame();
					}

					//mouse click position is in the save button
					if (button[1].getGlobalBounds().contains(sf::Vector2f(localPosition))) {

						soundSourceButton.setBuffer(buttonSound);
						soundSourceButton.play();

						writeSaveFile.open(targetFile);
						if (writeSaveFile.is_open()) {

							writeSaveFile << brickColumns << " " << brickRows << " " << maxBrickHealth << "\n";

							for (int i = 0; i < brickColumns; i++) {
								for (int j = 0; j < brickRows; j++) {
									writeSaveFile << brickID[i][j] << " ";
								}
							}

							writeSaveFile.close();
						}
						else {
							std::cout << "unable to open file" << std::endl;
						}
					}

					//mouse click position is in the reset button
					if (button[2].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
						LoadLevel();
					}

					//mouse click position is in the random button
					if (button[9].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
						for (int i = 0; i < brickColumns; i++) {
							for (int j = 0; j < brickRows; j++) {
								brickID[i][j] = 1 + rand() % (2 * (numberOfBrickTypes));
								isCollidable[i][j] = brickID[i][j] % 2;
								//isCollidable[i][j] = rand() % 2;
							}
						}
					}

					//mouse click position is in the back button
					if (button[10].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
						isEditor = false;

						soundSourceButton.setBuffer(backButtonSound);
						soundSourceButton.play();
					}

					//mouse click position is in the + COL button
					if (button[3].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
						if (brickColumns < maxBrickColumns) {
							brickColumns++;

							CreateBrickArray();
						}
					}

					//mouse click position is in the - COL button
					if (button[4].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
						if (brickColumns > minBrickColumns) {
							brickColumns--;

							CreateBrickArray();
						}
					}

					//mouse click position is in the + ROW button
					if (button[5].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
						if (brickRows < maxBrickRows) {
							brickRows++;

							CreateBrickArray();
						}
					}

					//mouse click position is in the - ROW button
					if (button[6].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
						if (brickRows > minBrickRows) {
							brickRows--;

							CreateBrickArray();
						}
					}

					//mouse click position is in the + BRICK HEALTH button
					if (button[7].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
						if (maxBrickHealth < defaultBrickHealth) {
							maxBrickHealth++;

							CreateBrickArray();
						}
					}

					//mouse click position is in the - BRICK HEALTH button
					if (button[8].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
						if (maxBrickHealth > 1) {
							maxBrickHealth--;

							CreateBrickArray();
						}
					}
				}
			}
			else if (isEditor == false) {
				
				//the menu inputs
				if (isGame == false) {

					if (instructionsOn == false) {
						//mouse click position is in the LEVEL 1
						if (menuButtons[0][0].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
							targetFile = level1Save;

							soundSourceButton.setBuffer(buttonSound);
							soundSourceButton.play();

							LoadLevel();

							isGame = true;
							StartGame();
						}

						//mouse click position is in the LEVEL 2
						if (menuButtons[0][1].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
							targetFile = level2Save;

							soundSourceButton.setBuffer(buttonSound);
							soundSourceButton.play();

							LoadLevel();

							isGame = true;
							StartGame();
						}

						//mouse click position is in the LEVEL 3
						if (menuButtons[0][2].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
							targetFile = level3Save;

							soundSourceButton.setBuffer(buttonSound);
							soundSourceButton.play();

							LoadLevel();

							isGame = true;
							StartGame();
						}

						//mouse click position is in the CUSTOM 1
						if (menuButtons[1][0].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
							targetFile = custom1Save;

							soundSourceButton.setBuffer(buttonSound);
							soundSourceButton.play();

							LoadLevel();
							isEditor = true;
						}

						//mouse click position is in the CUSTOM 2
						if (menuButtons[1][1].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
							targetFile = custom2Save;

							soundSourceButton.setBuffer(buttonSound);
							soundSourceButton.play();

							LoadLevel();
							isEditor = true;
						}

						//mouse click position is in the CUSTOM 3
						if (menuButtons[1][2].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
							targetFile = custom3Save;

							soundSourceButton.setBuffer(buttonSound);
							soundSourceButton.play();

							LoadLevel();
							isEditor = true;
						}

						//mouse click position is in the intructions button
						if (instructionButton.getGlobalBounds().contains(sf::Vector2f(localPosition))) {
							instructionsOn = true;

							soundSourceButton.setBuffer(buttonSound);
							soundSourceButton.play();
						}
					}
					else {
						//mouse click position is in the back button
						if (button[10].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
							instructionsOn = false;

							soundSourceButton.setBuffer(backButtonSound);
							soundSourceButton.play();
						}
					}
				}
			}
		}			

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			
			if (isEditor == true && isGame == false) {
				//setting the brick type
				for (int i = 0; i < brickColumns; i++) {
					for (int j = 0; j < brickRows; j++) {
						if (bricks[i][j].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
							if (brickID[i][j] < 2 * numberOfBrickTypes - 1) {
								brickID[i][j] += 2;
							}
							else {
								brickID[i][j] = 1;
							}
						}
					}
				}
			}			
		}		
	}

	//other keyboard, mouse events
	
}

void App::Run() {
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			HandleEvents();
		}
		Update();
		Draw();
	}
}

void App::LoadLevel() {
	readSaveFile.open(targetFile);
	if (readSaveFile.is_open()) {

		readSaveFile >> brickColumns;
		readSaveFile >> brickRows;
		readSaveFile >> maxBrickHealth;

		CreateBrickArray();

		while (!readSaveFile.eof()) {
			for (int i = 0; i < brickColumns; i++) {
				for (int j = 0; j < brickRows; j++) {
					readSaveFile >> brickID[i][j];

					isCollidable[i][j] = brickID[i][j] % 2;
					brickHealth[i][j] = maxBrickHealth;
				}
			}
		}

		readSaveFile.close();
	}
	else {
		std::cout << "unable to open file" << std::endl;
	}
}

void App::CreateBrickArray() {
	
	if (window.getSize().x - 2 * yBrickPad < (brickColumns - 1) * brickGap + brickColumns * defaultBrickWidth) {
		brickWidth = (window.getSize().x - 2 * yBrickPad - (brickColumns - 1) * brickGap) / brickColumns;
	}
	else {
		brickWidth = defaultBrickWidth;
	}

	if (40 * maxBrickRows + (maxBrickRows - 1) * brickGap < (brickRows - 1) * brickGap + brickRows * defaultBrickHeight) {
		brickHeight = ((40 * maxBrickRows + (maxBrickRows - 1) * brickGap) - (brickRows - 1) * brickGap) / brickRows;
	}
	else {
		brickHeight = defaultBrickHeight;
	}
	
	xBrickPad = (window.getSize().x - brickColumns * brickWidth - (brickColumns - 1) * brickGap) / 2;

	// dynamically allocate memory for an array of pointers to various arrays
	bricks = new sf::RectangleShape* [brickColumns];
	isCollidable = new bool* [brickColumns];
	brickHealth = new int* [brickColumns];
	brickID = new int* [brickColumns];
	paddleUpgrade = new sf::CircleShape* [brickColumns];
	paralyzer = new sf::CircleShape* [brickColumns];
	paralyzerActive = new bool* [brickColumns];
	paralyzerLaunchAngle = new float* [brickColumns];
	// dynamically allocate memory for arrays of above arrays
	for (int i = 0; i < brickColumns; ++i) {
		bricks[i] = new sf::RectangleShape[brickRows];
		isCollidable[i] = new bool[brickRows];
		brickHealth[i] = new int[brickRows];
		brickID[i] = new int[brickRows];
		paddleUpgrade[i] = new sf::CircleShape[brickRows];
		paralyzer[i] = new sf::CircleShape[brickRows];
		paralyzerActive[i] = new bool[brickRows];
		paralyzerLaunchAngle[i] = new float[brickRows];
	}

	// initialise and output the 2D array of brick and their other attributes

	for (int i = 0; i < brickColumns; i++) {
		for (int j = 0; j < brickRows; j++) {
			bricks[i][j].setSize(sf::Vector2f(brickWidth, brickHeight));
			bricks[i][j].setOrigin(sf::Vector2f(0.5 * bricks[i][j].getSize().x, 0.5 * bricks[i][j].getSize().y));
			bricks[i][j].setPosition(xBrickPad + 0.5f * brickWidth + i * (brickWidth + brickGap), yBrickPad + 0.5f * brickHeight + j * (brickHeight + brickGap));
			
			brickID[i][j] = 1;
			isCollidable[i][j] = brickID[i][j] % 2;

			brickHealth[i][j] = maxBrickHealth;
						
			paddleUpgrade[i][j].setRadius(0.3f * bricks[i][j].getSize().y);
			paddleUpgrade[i][j].setPointCount(8);
			paddleUpgrade[i][j].setOrigin(sf::Vector2f(paddleUpgrade[i][j].getRadius(), paddleUpgrade[i][j].getRadius()));
			paddleUpgrade[i][j].setFillColor(paddleUpgradeColour);
			paddleUpgrade[i][j].setPosition(bricks[i][j].getPosition());

			paralyzer[i][j].setRadius(0.3f * bricks[i][j].getSize().y);
			paralyzer[i][j].setPointCount(3);
			paralyzer[i][j].setOrigin(sf::Vector2f(paddleUpgrade[i][j].getRadius(), paddleUpgrade[i][j].getRadius()));
			paralyzer[i][j].setFillColor(paralyzerColour);
			paralyzer[i][j].setPosition(bricks[i][j].getPosition());
			paralyzerLaunchAngle[i][j] = 0;
			paralyzerActive[i][j] = false;
		}
	}	

	createdBricks = true;
}

void App::StartGame() {
	
	music.stop();
	if (!music.openFromFile("BattleTheme_stg_mus_v2.00.wav")) {
		std::cout << "BattleTheme music not loaded" << std::endl;
	}	
	music.setLoop(true);
	music.play();
	
	ballCount = maxBallCount;
	ballCountIcon.setFillColor(brickFullHealthColour);
	ballCountText.setString(std::to_string(ballCount));

	initialLanchAngle = (rand() % 120 + 30) * PI / 180;
	std::cout << "intialLaunchAngle = " << initialLanchAngle * 180 / PI << std::endl;

	ballSpeed = 500.0f;

	xBallSpeed = ballSpeed * cos(initialLanchAngle);
	//std::cout << "xSpeed = " << xSpeed << std::endl;

	yBallSpeed = -ballSpeed * sin(initialLanchAngle);
	//std::cout << "ySpeed = " << ySpeed << std::endl;

	colouredBricksRemaining = 0;

	for (int i = 0; i < brickColumns; i++) {
		for (int j = 0; j < brickRows; j++) {
			if (brickID[i][j] == 1 || brickID[i][j] == paddleUpgradeID) {
				colouredBricksRemaining++;
			}
		}
	}

	std::cout << colouredBricksRemaining << std::endl;
}
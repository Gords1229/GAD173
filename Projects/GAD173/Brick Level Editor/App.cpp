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

	for (int i = 0; i < brickColumns; ++i) {
		delete[] bricks[i];
		delete[] isCollidable[i];
		delete[] brickHealth[i];
		delete[] brickID[i];
		delete[] paddleUpgrade[i];
	}

	delete[] bricks;
	delete[] isCollidable;
	delete[] brickHealth;
	delete[] brickID;
	delete[] paddleUpgrade;
}

bool App::Init() {
	srand(time(NULL));
	
	//initialise App data members
	font.loadFromFile("arial.ttf");
	isGame = false;

	//initialise sound
	if (!brickDestroyed.loadFromFile("AlanOw!.wav")) {
		std::cout << "AlanOw! sound not loaded"<<std::endl;
	}
		
	if (!paddleHit.loadFromFile("ArrowShot_itm_sfx_v1.00.wav")) {
		std::cout << "ArrowShot_itm_sfx_v1.00 sound not loaded"<<std::endl;
	}

	//intiialise timers
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

	//initialise ball
	
	PI = 2 * asin(1);

	ballRadius = 10.0f;
	ball.setRadius(ballRadius);

	ball.setOrigin(sf::Vector2f(ball.getRadius(), ball.getRadius()));

	initialBallPosition = sf::Vector2f(window.getSize().x * 0.5f, paddle.getPosition().y - 0.5f * paddle.getSize().y - ballRadius);
	ball.setPosition(initialBallPosition);

	// initialise power ups and hazards
	numberOfBrickTypes = 2;
	powerUpFallSpeed = 200.0f;
	// all power up and hazard IDs must be odd numbers.
	paddleUpgradeID = 3;
	paddleUpgradeColour = sf::Color::Blue;
	paddleUpgradeMaxHits = 10;
	paddleUpgradeHitCounter = 0;

	//initialise bricks
	//set bricks array size
	brickColumns = 8;
	minBrickColumns = 6;
	maxBrickColumns = 40;
	brickRows = 5;
	minBrickRows = 4;
	maxBrickRows = 12;
	
	defaultBrickWidth = 200.0f;
	defaultBrickHeight = 50.0f;
	brickGap = 3.0f;
	yBrickPad = 100.0f;

	defaultBrickHealth = 3;
	maxBrickHealth = defaultBrickHealth;
	brickFullHealthColour = sf::Color::Green;
	brickDamagedHealthColour = sf::Color::Yellow;
	brickCriticalHealthColour = sf::Color::Red;

	CreateBrickArray();

	//intialise buttons
	numberOfButtons = 11;
	buttonNames = new std::string[numberOfButtons]{
		"START", "SAVE", "LOAD", "+ COL", "- COL", "+ ROW", "- ROW", "+ BRICK HEALTH", "- BRICK HEALTH", "RANDOMIZE", "RESET"
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

	//initialise collision variables

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

		//ball motion
		ball.move(xBallSpeed * deltaTime, yBallSpeed * deltaTime);
	}

	//game logic

	//left collision border detection
	if (ball.getPosition().x < ballRadius) {
		ball.setPosition(ballRadius, ball.getPosition().y);
		xBallSpeed = -xBallSpeed;
	}

	//right collision border detection
	if (ball.getPosition().x > window.getSize().x - ballRadius) {
		ball.setPosition(window.getSize().x - ballRadius, ball.getPosition().y);
		xBallSpeed = -xBallSpeed;
	}

	//top collision border detection
	if (ball.getPosition().y < ballRadius) {
		ball.setPosition(ball.getPosition().x, ballRadius);
		yBallSpeed = -yBallSpeed;
	}

	//bottom collision border detection
	if (ball.getPosition().y > window.getSize().y - ballRadius) {
		ball.setPosition(ball.getPosition().x, window.getSize().y - ballRadius);
		yBallSpeed = -yBallSpeed;
	}

	//brick collision
	for (int i = 0; i < brickColumns; i++) {
		for (int j = 0; j < brickRows; j++) {

			if (ball.getGlobalBounds().intersects(bricks[i][j].getGlobalBounds()) && isCollidable[i][j] == true) {

				brickHealth[i][j]--;

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

				sound.setBuffer(brickDestroyed);
				sound.play();

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

			//power up movement
			if (brickHealth[i][j] == 0) {
				if (brickID[i][j] == paddleUpgradeID) {
					paddleUpgrade[i][j].move(0.0f, powerUpFallSpeed * deltaTime);
				}
			}
		}
	}

	//paddle collision
	if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {

		sound.setBuffer(paddleHit);
		sound.play();

		if (paddleUpgradeHitCounter > 0) {
			paddleUpgradeHitCounter--;
		}
		else {
			paddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
			paddle.setOrigin(sf::Vector2f(0.5 * paddle.getSize().x, 0.5 * paddle.getSize().y));
		}

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

	//paddle upgrade collision
	for (int i = 0; i < brickColumns; i++) {
		for (int j = 0; j < brickRows; j++) {
			if (paddleUpgrade[i][j].getGlobalBounds().intersects(paddle.getGlobalBounds())) {
				//paddle.setScale(2.0f, 1.0f);
				paddleUpgrade[i][j].setPosition(-100, -100);
				paddle.setSize(sf::Vector2f(2 * paddleWidth, paddleHeight));
				paddle.setOrigin(sf::Vector2f(0.5 * paddle.getSize().x, 0.5 * paddle.getSize().y));
				paddleUpgradeHitCounter = paddleUpgradeMaxHits;
				brickID[i][j] = 0;

				std::cout << "paddle upgrade collected" << std::endl;
			}
		}
	}
}

void App::Draw() {
	window.clear();
	window.setView(view);

	//draw
	window.draw(ball);
	window.draw(paddle);
	window.draw(frameTime);	

	if (isGame == false) {
		
		window.draw(totalTime);
		
		for (int i = 0; i < numberOfButtons; i++) {
			window.draw(button[i]);
			window.draw(buttonText[i]);
		}		
	}	

	for (int i = 0; i < brickColumns; i++) {
		for (int j = 0; j < brickRows; j++) {
			
			if (isCollidable[i][j] == true) {
				window.draw(bricks[i][j]);				
			}

			if (brickID[i][j] == paddleUpgradeID) {
				window.draw(paddleUpgrade[i][j]);
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
		ballSpeed = 0.0f;
		ball.setPosition(initialBallPosition);
		paddle.setPosition(initialPaddlePosition);
		clockTime = 0.0f;
	}

	if (event.type == sf::Event::MouseButtonPressed/* && sf::Mouse::isButtonPressed(sf::Mouse::Left)*/) {

		//get the local mouse position (relative to a window)
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			
			//std::cout << "Left Mouse Pressed" << std::endl;

			//mouse click position is in the brick bounds
			for (int i = 0; i < brickColumns; i++) {
				for (int j = 0; j < brickRows; j++) {
					if (bricks[i][j].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
						isCollidable[i][j] = !isCollidable[i][j];
					}
				}
			}

			if (isGame == false) {

				//mouse click position is in the start button
				if (button[0].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
					isGame = true;

					initialLanchAngle = (rand() % 120 + 30) * PI / 180;
					std::cout << "intialLaunchAngle = " << initialLanchAngle * 180 / PI << std::endl;

					ballSpeed = 500.0f;

					xBallSpeed = ballSpeed * cos(initialLanchAngle);
					//std::cout << "xSpeed = " << xSpeed << std::endl;

					yBallSpeed = -ballSpeed * sin(initialLanchAngle);
					//std::cout << "ySpeed = " << ySpeed << std::endl;
				}

				//mouse click position is in the save button
				if (button[1].getGlobalBounds().contains(sf::Vector2f(localPosition))) {

					writeSaveFile.open("brickEditorSavaData.txt");
					if (writeSaveFile.is_open()) {

						for (int i = 0; i < brickColumns; i++) {
							for (int j = 0; j < brickRows; j++) {
								writeSaveFile << isCollidable[i][j] << " ";
							}
						}

						writeSaveFile.close();
					}
					else {
						std::cout << "unable to open file" << std::endl;
					}
				}

				//mouse click position is in the load button
				if (button[2].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
					readSaveFile.open("brickEditorSavaData.txt");
					if (readSaveFile.is_open()) {

						while (!readSaveFile.eof()) {
							for (int i = 0; i < brickColumns; i++) {
								for (int j = 0; j < brickRows; j++) {
									readSaveFile >> isCollidable[i][j];
								}
							}
						}

						readSaveFile.close();
					}
					else {
						std::cout << "unable to open file" << std::endl;
					}
				}

				//mouse click position is in the random button
				if (button[9].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
					for (int i = 0; i < brickColumns; i++) {
						for (int j = 0; j < brickRows; j++) {
							isCollidable[i][j] = rand() % 2;
						}
					}
				}

				//mouse click position is in the reset button
				if (button[10].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
					for (int i = 0; i < brickColumns; i++) {
						for (int j = 0; j < brickRows; j++) {
							isCollidable[i][j] = true;
						}
					}
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

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
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
	// dynamically allocate memory for arrays of above arrays
	for (int i = 0; i < brickColumns; ++i) {
		bricks[i] = new sf::RectangleShape[brickRows];
		isCollidable[i] = new bool[brickRows];
		brickHealth[i] = new int[brickRows];
		brickID[i] = new int[brickRows];
		paddleUpgrade[i] = new sf::CircleShape[brickRows];
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
			if (brickHealth[i][j] == defaultBrickHealth) {
				bricks[i][j].setFillColor(brickFullHealthColour);
			}
			else if (brickHealth[i][j] == 1) {
				bricks[i][j].setFillColor(brickCriticalHealthColour);
			}
			else {
				bricks[i][j].setFillColor(brickDamagedHealthColour);
			}
			
			paddleUpgrade[i][j].setRadius(0.3f * bricks[i][j].getSize().y);
			paddleUpgrade[i][j].setPointCount(8);
			paddleUpgrade[i][j].setOrigin(sf::Vector2f(paddleUpgrade[i][j].getRadius(), paddleUpgrade[i][j].getRadius()));
			paddleUpgrade[i][j].setFillColor(paddleUpgradeColour);
			paddleUpgrade[i][j].setPosition(bricks[i][j].getPosition());

			
		}
	}
}
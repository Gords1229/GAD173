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

	delete[] bricks;
	delete[] isCollidable;
}

bool App::Init() {
	srand(time(NULL));
	
	//initialise App data members
	font.loadFromFile("arial.ttf");
	isplaying = false;

	//intiialise timers
	frameTime.setFont(font);
	frameTime.setCharacterSize(24);
	totalTime.setFont(font);
	totalTime.setCharacterSize(24);
	totalTime.setPosition(0, 34);
	
	//initialise paddle
	paddleWidth = 200.0f;
	paddleHeight = 50.0f;
	paddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	
	paddle.setOrigin(sf::Vector2f(0.5 * paddle.getSize().x, 0.5 * paddle.getSize().y));
	
	initialPaddlePosition = sf::Vector2f(window.getSize().x * 0.5f, window.getSize().y - 100.0f);
	paddle.setPosition(initialPaddlePosition);

	paddleSpeed = 500.0f;

	//initialise ball
	
	PI = 2 * asin(1);

	radius = 20.0f;
	ball.setRadius(radius);

	ball.setOrigin(sf::Vector2f(ball.getRadius(), ball.getRadius()));

	initialBallPosition = sf::Vector2f(window.getSize().x * 0.5f, paddle.getPosition().y - 0.5f * paddle.getSize().y - radius);
	ball.setPosition(initialBallPosition);
		
	//initialise bricks
	//set bricks array size
	brickColumns = 8;
	brickRows = 5;
	
	brickWidth = 200.0f;
	brickHeight = 50.0f;
	brickGap = 20.0f;
	yBrickPad = 100.0f;

	CreateBrickArray();

	//intiialise buttons
	numberOfButtons = 5;
	buttonNames = new std::string[numberOfButtons]{
		"START", "SAVE", "LOAD", "RANDOMIZE", "RESET"
	};

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

	button = new sf::RectangleShape[numberOfButtons];
	buttonText = new sf::Text[numberOfButtons];

	for (int i = 0; i < numberOfButtons; i++) {
		button[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
		button[i].setOrigin(sf::Vector2f(0.5f * button[i].getSize().x, 0.5f * button[i].getSize().y));		
		button[i].setPosition(window.getSize().x - (0.5f * buttonWidth + buttonGap) - (numberOfButtons - 1 - i) * (buttonWidth + buttonGap), 0.5f * buttonHeight);

		
		buttonText[i].setString(buttonNames[i]);
		buttonText[i].setFont(font);
		buttonText[i].setCharacterSize(0.8f * buttonHeight);
		buttonText[i].setOrigin(sf::Vector2f(0.5f * buttonText[i].getGlobalBounds().width, buttonText[i].getGlobalBounds().height));
		buttonText[i].setPosition(button[i].getPosition());		
		buttonText[i].setFillColor(buttonTextColour);

		std::cout << "button[" << i << "] position : text[" << i << "] position -  " << 
			button[i].getPosition().x << " , " << button[i].getPosition().y << " : " << 
			buttonText[i].getPosition().x << " , " << buttonText[i].getPosition().y << std::endl;

		if (buttonText[i].getGlobalBounds().width > 0.9f * buttonWidth) {
			buttonText[i].setScale((0.9f * buttonWidth) / buttonText[i].getGlobalBounds().width, (0.9f * buttonWidth) / buttonText[i].getGlobalBounds().width);
		}				
	}

	//initialise collision variables

	return true;
}

void App::Update() {
	//update
	deltaTime = clock.restart().asSeconds();
	frameTime.setString(std::to_string(deltaTime));
	//std::cout << deltaTime << std::endl;

	if (isplaying) {
		clockTime += deltaTime;
		totalTime.setString(std::to_string(clockTime));
		//std::cout << time << std::endl;

		//user Input

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			// A key is pressed: move our paddle left
			if (paddle.getPosition().x >= 0.5f * paddle.getSize().x) {
				paddle.move(-paddleSpeed * deltaTime, 0.f);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			// D key is pressed: move our paddle right
			if (paddle.getPosition().x <= window.getSize().x - 0.5f * paddle.getSize().x) {
				paddle.move(paddleSpeed * deltaTime, 0.f);
			}
		}

		//ball motion
		ball.move(xSpeed * deltaTime, ySpeed * deltaTime);
	}		
	
	//game logic

	//left collision border detection
	if (ball.getPosition().x < radius) {
		ball.setPosition(radius, ball.getPosition().y);
		xSpeed = -xSpeed;
	}
	
	//right collision border detection
	if (ball.getPosition().x > window.getSize().x - radius) {
		ball.setPosition(window.getSize().x - radius, ball.getPosition().y);
		xSpeed = -xSpeed;
	}

	//top collision border detection
	if (ball.getPosition().y < radius) {
		ball.setPosition(ball.getPosition().x, radius);
		ySpeed = -ySpeed;
	}

	//bottom collision border detection
	if (ball.getPosition().y > window.getSize().y - radius) {
		ball.setPosition(ball.getPosition().x, window.getSize().y - radius);
		ySpeed = -ySpeed;
	}	

	//brick collision
	for (int i = 0; i < brickColumns; i++) {
		for (int j = 0; j < brickRows; j++) {
			
			if (ball.getGlobalBounds().intersects(bricks[i][j].getGlobalBounds()) && isCollidable[i][j] == true) {
							
				isCollidable[i][j] = false;

				//determining the position of the collision
				colliderSideAngle = abs(atan2(radius + 0.5f * brickHeight, radius + 0.5f * brickWidth));
				collisionAngle = abs(atan2(ball.getPosition().y - bricks[i][j].getPosition().y, ball.getPosition().x - bricks[i][j].getPosition().x));

				if (collisionAngle < PI - colliderSideAngle && collisionAngle > colliderSideAngle) {
					ySpeed = -ySpeed;
				}
				else {
					xSpeed = -xSpeed;
				}
			}
		}
	}

	//paddle collision
	if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
		
		//determining the position of the collision
		colliderSideAngle = abs(atan2(radius + 0.5f * paddleHeight, radius + 0.5f * paddleWidth));
		collisionAngle = abs(atan2(ball.getPosition().y - paddle.getPosition().y, ball.getPosition().x - paddle.getPosition().x));

		if (collisionAngle < PI - colliderSideAngle && collisionAngle > colliderSideAngle) {
			//check which side the ball hit the paddle (top or bottom) and adjust the ball position accordingly
			if (ball.getPosition().y > paddle.getPosition().y) {
				ball.setPosition(ball.getPosition().x, ball.getPosition().y + 0.5f * paddle.getSize().y + radius);
			}
			else {
				ball.setPosition(ball.getPosition().x, ball.getPosition().y - 0.5f * paddle.getSize().y + radius);
			}
			ySpeed = -ySpeed;
		}
		else {
			//check which side the ball hit the paddle (left or right) and adjust the ball position accordingly
			if (ball.getPosition().x > paddle.getPosition().x) {
				ball.setPosition(paddle.getPosition().x + 0.5f * paddle.getSize().x + paddleSpeed * deltaTime + radius, ball.getPosition().y);
			}
			else {
				ball.setPosition(paddle.getPosition().x - 0.5f * paddle.getSize().x + paddleSpeed * deltaTime + radius, ball.getPosition().y);
			}
						
			xSpeed = -xSpeed;
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

	if (isplaying == false) {
		
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
		}
	}

	window.display();
}

void App::HandleEvents() {
	if (event.type == sf::Event::Closed) {
		window.close();
	}

	if (isplaying == true && sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		// R key is pressed: reset back to editor
		isplaying = false;
		ballSpeed = 0.0f;
		ball.setPosition(initialBallPosition);
		paddle.setPosition(initialPaddlePosition);
		clockTime = 0.0f;
	}

	if (event.type == sf::Event::MouseButtonPressed/* && sf::Mouse::isButtonPressed(sf::Mouse::Left)*/) {

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			//get the local mouse position (relative to a window)
			sf::Vector2i localPosition = sf::Mouse::getPosition(window);
			//std::cout << "Left Mouse Pressed" << std::endl;

			//check to see if the mouse click position is in the brick bounds
			for (int i = 0; i < brickColumns; i++) {
				for (int j = 0; j < brickRows; j++) {
					if (bricks[i][j].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
						isCollidable[i][j] = !isCollidable[i][j];
					}
				}
			}

			if (isplaying == false) {

				//mouse click position is in the start button
				if (button[0].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
					isplaying = true;

					initialLanchAngle = (rand() % 120 + 30) * PI / 180;
					std::cout << "intialLaunchAngle = " << initialLanchAngle * 180 / PI << std::endl;

					ballSpeed = 500.0f;

					xSpeed = ballSpeed * cos(initialLanchAngle);
					//std::cout << "xSpeed = " << xSpeed << std::endl;

					ySpeed = -ballSpeed * sin(initialLanchAngle);
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
				if (button[3].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
					for (int i = 0; i < brickColumns; i++) {
						for (int j = 0; j < brickRows; j++) {
							isCollidable[i][j] = rand() % 2;
						}
					}
				}

				//mouse click position is in the reset button
				if (button[4].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
					for (int i = 0; i < brickColumns; i++) {
						for (int j = 0; j < brickRows; j++) {
							isCollidable[i][j] = true;
						}
					}
				}

			}			
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			std::cout << "Right Mouse Pressed" << std::endl;
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
	
	xBrickPad = (window.getSize().x - brickColumns * brickWidth - (brickColumns - 1) * brickGap) / 2;

	// dynamically allocate memory for an array of pointers to sf::RectangleShape & Booleans
	bricks = new sf::RectangleShape * [brickColumns];
	isCollidable = new bool* [brickColumns];
	// dynamically allocate memory for arrays of sf::RectangleShape & Booleans
	for (int i = 0; i < brickColumns; ++i) {
		bricks[i] = new sf::RectangleShape[brickRows];
		isCollidable[i] = new bool[brickRows];
	}

	// initialise and output the 2D array of brick and their isCollidable booleans


	for (int i = 0; i < brickColumns; i++) {
		for (int j = 0; j < brickRows; j++) {
			bricks[i][j].setSize(sf::Vector2f(brickWidth, brickHeight));
			bricks[i][j].setOrigin(sf::Vector2f(0.5 * bricks[i][j].getSize().x, 0.5 * bricks[i][j].getSize().y));
			bricks[i][j].setPosition(xBrickPad + 0.5f * brickWidth + i * (brickWidth + brickGap), yBrickPad + 0.5f * brickHeight + j * (brickHeight + brickGap));
			isCollidable[i][j] = true;
		}
	}
}
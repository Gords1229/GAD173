#include <iostream>
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
}

bool App::Init() {
	srand(time(NULL));
	
	//initialise App data members
	font.loadFromFile("arial.ttf");
	frameTime.setFont(font);
	frameTime.setCharacterSize(24);
	totalTime.setFont(font);
	totalTime.setCharacterSize(24);
	totalTime.setPosition(0, 34);
		

	//initialise ball
	
	int sign = 2 * (rand() % 2) - 1;	

	radius = 20.0f;
	circle.setRadius(radius);
	circle.setPosition(window.getSize().x * 0.5f - radius, window.getSize().y  * 0.8f - radius);

	PI = 2 * asin(1);
	initialLanchAngle = (rand() % 120 + 30) * 180 / PI;

	speed = 500.0f;

	xSpeed = speed * cos(initialLanchAngle);
	std::cout << "xSpeed = " << xSpeed << std::endl;

	ySpeed = -speed * sin(initialLanchAngle);
	std::cout << "ySpeed = " << ySpeed << std::endl;
	
	//initialLaunchVector = sf::Vector2f(xSpeed, ySpeed);	

	//initialise paddle
	paddleWidth = 200.0f;
	paddleHeight = 50.0f;
	yPaddlePosition = 100.0f;
	paddleSpeed = 500.0f;

	paddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
	paddle.setPosition(window.getSize().x * 0.5f + radius, window.getSize().y - yPaddlePosition);

	//initialise bricks
	brickWidth = 200.0f;
	brickHeight = 50.0f;
	brickGap = 20.0f;
	xBrickPad = (window.getSize().x - brickCol * brickWidth - (brickCol -1) * brickGap) / 2;
	yBrickPad = 100.0f;
	

	for (int i = 0; i < brickCol; i++) {
		for (int j = 0; j < brickRow; j++) {
			bricks[i][j].setSize(sf::Vector2f(brickWidth, brickHeight));
			bricks[i][j].setPosition(xBrickPad + i * (brickWidth + brickGap), yBrickPad + j * (brickHeight + brickGap));
			isCollidable[i][j] = true;
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

	clockTime += deltaTime;
	totalTime.setString(std::to_string(clockTime));
	//std::cout << time << std::endl;
	
	//user Input

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		// left key is pressed: move our paddle left
		if (paddle.getPosition().x >= 0) {
			paddle.move(-paddleSpeed * deltaTime, 0.f);
		}		
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		// right key is pressed: move our paddle right
		if (paddle.getPosition().x <= window.getSize().x - paddleWidth) {
			paddle.move(paddleSpeed * deltaTime, 0.f);
		}
	}
	
	//game logic

	//left collision border detection
	if (circle.getPosition().x < 0) {
		circle.setPosition(0, circle.getPosition().y);
		xSpeed = -xSpeed;
	}
	
	//right collision border detection
	if (circle.getPosition().x > window.getSize().x - 2 * radius) {
		circle.setPosition(window.getSize().x - 2 * radius, circle.getPosition().y);
		xSpeed = -xSpeed;
	}

	//top collision border detection
	if (circle.getPosition().y < 0) {
		circle.setPosition(circle.getPosition().x, 0);
		ySpeed = -ySpeed;
	}

	//bottom collision border detection
	if (circle.getPosition().y > window.getSize().y - 2 * radius) {
		circle.setPosition(circle.getPosition().x, window.getSize().y - 2 * radius);
		ySpeed = -ySpeed;
	}
	
	//ball motion
	circle.move(xSpeed * deltaTime, ySpeed * deltaTime);

	//brick collision
	for (int i = 0; i < brickCol; i++) {
		for (int j = 0; j < brickRow; j++) {
			
			if (circle.getGlobalBounds().intersects(bricks[i][j].getGlobalBounds()) && isCollidable[i][j] == true) {
							
				isCollidable[i][j] = false;

				//determining the position of the collision
				colliderSideAngle = abs(atan2(radius + brickHeight / 2, radius + brickWidth / 2));
				ballCentre = sf::Vector2f(circle.getPosition().x + radius, circle.getPosition().y + radius);
				colliderCenter = sf::Vector2f(bricks[i][j].getPosition().x + radius, bricks[i][j].getPosition().y + radius);
				collisionAngle = abs(atan2(ballCentre.y - colliderCenter.y, ballCentre.x - colliderCenter.x));

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
	if (paddle.getGlobalBounds().intersects(circle.getGlobalBounds())) {
		
		//determining the position of the collision
		colliderSideAngle = abs(atan2(radius + brickHeight / 2, radius + brickWidth / 2));
		ballCentre = sf::Vector2f(circle.getPosition().x + radius, circle.getPosition().y + radius);
		colliderCenter = sf::Vector2f(paddle.getPosition().x + radius, paddle.getPosition().y + radius);
		collisionAngle = abs(atan2(ballCentre.y - colliderCenter.y, ballCentre.x - colliderCenter.x));

		if (collisionAngle < PI - colliderSideAngle && collisionAngle > colliderSideAngle) {
			ySpeed = -ySpeed;
		}
		else {
			xSpeed = -xSpeed;
		}
	}

}

void App::Draw() {
	window.clear();
	window.setView(view);

	//draw
	window.draw(circle);
	window.draw(paddle);
	window.draw(frameTime);
	window.draw(totalTime);

	for (int i = 0; i < brickCol; i++) {
		for (int j = 0; j < brickRow; j++) {
			
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
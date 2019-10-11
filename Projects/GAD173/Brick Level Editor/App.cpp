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

	radius = 50.0f;
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
		}		
	}

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
		xSpeed = -xSpeed;
	}
	
	//right collision border detection
	if (circle.getPosition().x > window.getSize().x - 2 * radius) {
		xSpeed = -xSpeed;
	}

	//top collision border detection
	if (circle.getPosition().y < 0) {
		ySpeed = -ySpeed;
	}

	//bottom collision border detection
	if (circle.getPosition().y > window.getSize().y - 2 * radius) {
		ySpeed = -ySpeed;
	}

	circle.move(xSpeed * deltaTime, ySpeed * deltaTime);
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
			window.draw(bricks[i][j]);
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
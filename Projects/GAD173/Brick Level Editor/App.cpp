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
	//initialise App data members
	font.loadFromFile("arial.ttf");
	frameTime.setFont(font);
	frameTime.setCharacterSize(24);
	totalTime.setFont(font);
	totalTime.setCharacterSize(24);
	totalTime.setPosition(0, 34);
		
	radius = 10.0f;
	circle.setRadius(radius);
	circle.setPosition(window.getSize().x / 2, window.getSize().y / 2);

	xSpeed = 1000.0f;
	ySpeed = 1000.0f;	

	return true;
}

void App::Update() {
	//update
	deltaTime = clock.restart().asSeconds();
	frameTime.setString(std::to_string(deltaTime));
	//std::cout << deltaTime << std::endl;

	time += deltaTime;
	totalTime.setString(std::to_string(time));
	//std::cout << time << std::endl;
	
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
	window.draw(frameTime);
	window.draw(totalTime);

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
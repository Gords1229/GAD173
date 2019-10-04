#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Hello SFML World!");
	sf::CircleShape circle(100.0f);

	// main game loop
	while (window.isOpen())
	{

		// user input
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();

		// game logic

		// rendering
		window.draw(circle);
		window.display();
	}
	return EXIT_SUCCESS;
}
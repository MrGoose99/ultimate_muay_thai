#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Game.hpp"
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace sf;

int main()
{
	sf::RenderWindow window(VideoMode({ 1920, 1080 }), "Ultimate_Muay_Thai");
	Game game;
	Clock clock;
	clock.start();
	Time dt;
	srand(time(NULL));
	while (window.isOpen())
	{
		dt = clock.restart(); // clock is restarting every frame
		while (const optional<Event> event = window.pollEvent()) //events checking (can't be a methode because of SFML limitations)
		{
			if (game.get_status() == 1)
			{
				game.checkEvents_running(event, window, dt);
			}	
			else
			{
				game.checkEvents_paused(event, window);
			}
		}
		if (game.get_status() == 1)
		{
			game.animations_update(dt); //checking animations update every frame
			game.position_update(dt); //checking positions every frame
		}
		window.clear(sf::Color::Black);
		game.draw(window);
		window.display();
	}
	return 0;
}
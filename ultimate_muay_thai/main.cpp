#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Game.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>

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
		if (dt >= sf::seconds(0.05f)) dt = sf::seconds(0.f); //prevent to SUM dt when game freezes
		while (const optional<Event> event = window.pollEvent()) //events checking (can't be a methode because of SFML limitations)
		{
			if (game.get_status() == 1) //running
			{
				game.checkEvents_running(event, window, dt);
			}	
			else if (game.get_status() == 0) //paused
			{
				game.checkEvents_paused(event, window);
			}
			else if (game.get_status() == 2) //main_menu
			{
				game.checkEvents_main_menu(event, window);
			}
		}
		if (game.get_status() != 0)
		{
			game.position_update(dt); //checking positions every frame
			game.animations_update(dt); //checking animations update every frame
		}
		window.clear(sf::Color::Black);
		game.draw(window);
		window.display();
	}
	return 0;
}
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Game.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "Game_status.hpp"

using namespace std;
using namespace sf;

int main()
{
	sf::RenderWindow window(VideoMode({ 1920, 1080 }), "Ultimate_Muay_Thai", State::Windowed);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);
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
			if (event->is<sf::Event::Closed>()) // closing the window by every possible way (but not from keyboard)
				window.close();
			if (game.get_status() == Game_status::Running) //running
			{
				game.checkEvents_running(event, window, dt);
			}	
			else if (game.get_status() == Game_status::Paused) //paused
			{
				game.checkEvents_paused(event, window);
			}
			else if (game.get_status() == Game_status::Main_menu) //main_menu
			{
				game.checkEvents_main_menu(event, window);
			}
			else if (game.get_status() == Game_status::In_game_menu) //in game menu
			{
				game.checkEvents_in_game_menu(event, window);
			}
		}
		if (game.get_status() == Game_status::Running || game.get_status() == Game_status::Main_menu || game.get_status() == Game_status::Cutscene || game.get_status() == Game_status::Game_over)
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
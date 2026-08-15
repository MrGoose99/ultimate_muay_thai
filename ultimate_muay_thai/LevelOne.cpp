#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "LevelOne.hpp"
#include <iostream>
#include <vector>
#include "TileMap.hpp"
#include <array>
#include <cstdlib>
#include "Interactive.hpp"
#include "Gem.hpp"
#include "Spiked_roller.hpp"
#include "Punching_bag.hpp"
#include "Moving_tile.hpp"
#include "include/json.hpp"
#include <fstream>
#include "Character.hpp"
#include "Player.hpp"

using json = nlohmann::json;

LevelOne::LevelOne(Player& p1)
{
	background_shape.setSize({ 1920,1080 });
	background_shape.setTexture(&background);
	background_shape.setPosition({ 0,0 });

	LevelOne::load_level("json/level_test.json"); //TESTING LEVEL

	LevelOne::tilemap.set_tilemap("tilesets/tileset_lvl_1.png", { 128,128 }, tilemap_array.data(), width, height);

	camera.setSize({ 1920,1080 });
	
	for (short int i = 0; i < collision_array.size(); i++)
	{
		if (tilemap_array[i] > 0 && tilemap_array[i] <= 13)
			collision_array[i] = 1;
		else collision_array[i] = 0;
	}

	for (auto& int_obj : interactive_objects)
	{
		int tile = int_obj->get_current_tile();
		if (tile < 0 || tile >= interactive_grid.size())
		{
			std::cout << "OUT OF BOUNDS! Tile value =  " << tile << std::endl;

		}
		else
			interactive_grid[tile].push_back(int_obj.get());
	}
	p1.set_character_position(192.f, 192.f);
	std::cout << "STARTING INT_GRID SIZE: " << interactive_grid.size() << std::endl;
}

void LevelOne::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= Transformable::getTransform(); //applying the transform of the level
	target.setView(target.getDefaultView());
	target.draw(background_shape, states); //drawing the background of the level
	target.setView(camera);
	target.draw(tilemap, states); //drawing the tilemap of the level
	for (int i = 0; i < interactive_objects.size(); i++) //drawing the interactive objects of the level
	{
		if (interactive_objects[i]->get_status())target.draw(*interactive_objects[i], states);
	}
	/*for (int i = 0; i < character_grid.size(); i++) //ATTACKBOX FOR DEBUGGING
		for (int j = 0; j < character_grid[i].size(); j++)
		{
			target.draw(character_grid[i][j]->get_debug_2_shape(), states);
		}*/
}




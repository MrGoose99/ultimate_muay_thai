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

using json = nlohmann::json;

LevelOne::LevelOne()
{
	background_shape.setSize({ 1920,1080 });
	background_shape.setTexture(&background);
	background_shape.setPosition({ 0,0 });

	LevelOne::load_level("json/level_one.json");

	LevelOne::tilemap.set_tilemap("tilesets/tileset_lvl_1.png", { 128,128 }, tilemap_array.data(), width, height);

	collision_array.resize(tiles_in_level);

	interactive_grid.resize(tiles_in_level);

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
		interactive_grid[tile].push_back(int_obj.get());
	}
}

void LevelOne::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= Transformable::getTransform(); //applying the transform of the level
	target.setView(target.getDefaultView());
	target.draw(background_shape, states); //drawing the background of the level
	target.setView(camera);
	target.draw(tilemap, states); //drawing the tilemap of the level
	for (int i = 0; i < interactive_objects.size(); i++) //drawing the interactive objects of the level
		if (interactive_objects[i]->get_status())target.draw(*interactive_objects[i], states);
}




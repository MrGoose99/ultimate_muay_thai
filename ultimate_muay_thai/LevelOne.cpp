#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "LevelOne.hpp"
#include <iostream>
#include <vector>
#include "TileMap.hpp"
#include <array>
#include "Interactive.hpp"
#include "Gem.hpp"
#include "Spiked_roller.hpp"
#include "Punching_bag.hpp"



LevelOne::LevelOne()
{
	background_shape.setSize({ 1920,1080 });
	background_shape.setTexture(&background);
	background_shape.setPosition({ 0,0 });

	tilemap.set_tilemap("tilesets/tileset_lvl_1.png", { 128,128 }, tilemap_array.data(), 16, 8);

	tiles_in_row = 16; //TO CHANGE LATER

	camera.setSize({ 1920,1080 });

	for (short int i = 0; i < collision_array.size(); i++)
	{
		if (tilemap_array[i] > 0 && tilemap_array[i] <= 13)
			collision_array[i] = 1;
		else collision_array[i] = 0;
	}

	for (short int i = 0; i < interactive_array.size(); i++)
	{
		switch (interactive_array[i])
		{
		case 0:
			interactive_objects.push_back(std::make_unique<Gem>(0, i, 0));
			break;
		case 1:
			interactive_objects.push_back(std::make_unique<Gem>(0, i));
			break;

		case 2:
			interactive_objects.push_back(std::make_unique<Gem>(1, i));
			break;

		case 3:
			interactive_objects.push_back(std::make_unique<Spiked_roller>(i));
			break;

		case 4:
			interactive_objects.push_back(std::make_unique<Punching_bag>(i));
			break;

		default:
			break;
		}
	}
}

void LevelOne::set_lvl_tiles()
{
	

}

std::array<bool, 128> LevelOne::get_collision_array()
{
	return collision_array;
}

void LevelOne::update_interactive_objects(sf::Time& dt)
{
	for (const auto& interactive_object : interactive_objects)
	{
		if (interactive_object->get_object_type() == "hp_gem" || interactive_object->get_object_type() == "special_gem")
			interactive_object->update(dt, 10.f);
		if (interactive_object->get_punched() > 0)interactive_object->update_punched(dt);
		interactive_object->texture_update(dt);
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
	{
		if(interactive_objects[i]->get_status())target.draw(*interactive_objects[i], states);
	}

}

std::vector<std::unique_ptr<Interactive>>& LevelOne::get_interactive_objects()
{
	return interactive_objects;
}
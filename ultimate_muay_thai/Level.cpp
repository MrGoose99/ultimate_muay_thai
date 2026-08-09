#include "Level.hpp"
#include <iostream>
#include "TileMap.hpp"
#include <SFML/Graphics.hpp>
#include "Interactive.hpp"
#include "Gem.hpp"
#include "Spiked_roller.hpp"
#include "Punching_bag.hpp"
#include "Moving_tile.hpp"
#include "Spikes.hpp"
#include <fstream>
#include "include/json.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"


using json = nlohmann::json;


void Level::set_lvl_background()
{
	background_shape.setSize({ 1920,1080 });
	background_shape.setTexture(&background);
	background_shape.setPosition({ 0,0 });
}

sf::RectangleShape& Level::get_background()
{
	return background_shape;
}

TileMap& Level::get_tilemap()
{
	return tilemap;
}

void Level::set_camera_center(sf::Vector2f center_pos)
{
	camera.setCenter({ round(center_pos.x), round(center_pos.y) });
}

const int Level::get_tiles_in_row()
{
	return tiles_in_row;
}

void Level::load_level(const std::string& path)
{
	tilemap_array.clear();
	interactive_objects.clear();
	interactive_grid.clear();
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cout << "Error loading level from json";
		return;
	}

	json level_data;
	file >> level_data;
	file.close();

	tiles_in_row = level_data["level_size"]["w"].get<int>();
	height = level_data["level_size"]["h"].get<int>();
	width = level_data["level_size"]["w"].get<int>();
	tiles_in_level = height * width;
	interactive_grid.resize(tiles_in_level);
	collision_array.resize(tiles_in_level);
	character_grid.resize(tiles_in_level);
	for (auto& tile : level_data["tiles"])
		tilemap_array.push_back(tile);
	for (auto& obj : level_data["objects"])
	{
		int tile_x = obj["x"].get<int>();
		int tile_y = obj["y"].get<int>();
		int tile = tile_x + tile_y * tiles_in_row;

		if (obj["type"] == "gem")
			interactive_objects.push_back(std::make_unique<Gem>(obj["gem_status"].get<int>(), tile, tiles_in_row, 1));
		else if (obj["type"] == "spiked_roller")
			interactive_objects.push_back(std::make_unique<Spiked_roller>(tile, tiles_in_row));
		else if (obj["type"] == "punching_bag")
			interactive_objects.push_back(std::make_unique<Punching_bag>(tile, tiles_in_row));
		else if (obj["type"] == "moving_tile")
			interactive_objects.push_back(std::make_unique<Moving_tile>(tile, obj["tiles"].get<int>(), obj["dir"].get<int>(), tiles_in_row));
		else if (obj["type"] == "spikes")
			interactive_objects.push_back(std::make_unique<Spikes>(tile, obj["dir"].get<int>(), tiles_in_row));
		else if (obj["type"] == "enemy")
			interactive_objects.push_back(std::make_unique<Enemy>(tile, interactive_grid, character_grid, tiles_in_row));
	}


}

void Level::update_interactive_objects(sf::Time& dt, sf::FloatRect& player_hitbox, sf::Vector2f& player_pos, Player& p1, int tiles_in_row)
{
	if (p1.get_is_shooting())
	{
		int tile;
		int tile_x = (p1.get_player_position().x + p1.get_character_hitbox().size.x) / 128.f;
		int tile_y = (p1.get_player_position().y + p1.get_character_hitbox().size.y / 128 * 52) / 128.f;
		tile = tile_x + tile_y * tiles_in_row;
		sf::Vector2f pos;
		if (p1.get_right_side())
			pos.x = p1.get_player_position().x + (p1.get_character_hitbox().size.x * 1.85f);
		else pos.x = p1.get_player_position().x;
		pos.y = p1.get_player_position().y + (p1.get_character_hitbox().size.y / 128 * 52);
		interactive_objects.push_back(std::make_unique<Bullet>(tile, p1.get_right_side(), tiles_in_row, pos));
		p1.set_is_shooting(false);
	}

	for (short int i = 0; i < interactive_objects.size(); i++)
	{
		short int tile = interactive_objects[i]->get_current_tile();
		if (interactive_objects[i]->get_object_type() == "hp_gem" || interactive_objects[i]->get_object_type() == "special_gem")
		{
			interactive_objects[i]->update(dt, 10.f, interactive_grid, interactive_objects, tiles_in_row, p1, collision_array, tiles_in_level, character_grid);
			if (interactive_objects[i]->get_destroyed())
			{
				interactive_grid[tile].erase(std::remove(interactive_grid[tile].begin(), interactive_grid[tile].end(), interactive_objects[i].get()), interactive_grid[tile].end());
				interactive_objects.erase(interactive_objects.begin() + i);
				i--;
				continue;
			}
		}
		else if (interactive_objects[i]->get_object_type() == "moving_tile")
			interactive_objects[i]->update(dt, 200.f, interactive_grid, interactive_objects, tiles_in_row, p1, collision_array, tiles_in_level, character_grid);
		else if (interactive_objects[i]->get_object_type() == "punching_bag")
		{
			if (interactive_objects[i]->get_destroyed())
			{
				interactive_grid[tile].erase(std::remove(interactive_grid[tile].begin(), interactive_grid[tile].end(), interactive_objects[i].get()), interactive_grid[tile].end());
				interactive_objects[i] = std::make_unique<Gem>(std::rand() % 2, interactive_objects[i]->get_current_tile(), tiles_in_row, 1);
				interactive_grid[tile].push_back(interactive_objects[i].get());
			}
		}
		else if (interactive_objects[i]->get_object_type() == "enemy")
		{

			interactive_objects[i]->update(dt, 200.f, interactive_grid, interactive_objects, tiles_in_row, p1, collision_array, tiles_in_level, character_grid); //in this update character_grid tiles are removing
			if (interactive_objects[i]->get_hp() <= 0 && !interactive_objects[i]->get_gem_spawned())
			{
				sf::Sprite obj_sprite = interactive_objects[i]->get_object_sprite();
				sf::Vector2f obj_pos = obj_sprite.getGlobalBounds().position;
				int tile = ((obj_pos.x + obj_sprite.getLocalBounds().size.x / 2) / 128.f) + (((obj_pos.y ) / 128.f) * tiles_in_row);
				interactive_objects.push_back(std::make_unique<Gem>(std::rand() % 2, tile, tiles_in_row, 1));
				interactive_grid[tile].push_back(interactive_objects[interactive_objects.size() - 1].get());
				interactive_objects[i]->set_gem_spawned(true);
			}
			if (interactive_objects[i]->get_destroyed())
			{
				std::vector<int> act_tiles = interactive_objects[i]->get_actual_tiles();
				for (auto& til : act_tiles)
				{
					interactive_grid[til].erase(std::remove(interactive_grid[til].begin(), interactive_grid[til].end(), interactive_objects[i].get()), interactive_grid[til].end());
				}
				interactive_objects.erase(interactive_objects.begin() + i);
				i--;
				continue;
			}
		}
		else if (interactive_objects[i]->get_object_type() == "bullet")
		{
			interactive_objects[i]->update(dt, 1600.f, interactive_grid, interactive_objects, tiles_in_row, p1, collision_array, tiles_in_level, character_grid);
			if (interactive_objects[i]->get_destroyed())
			{
				std::vector<int> act_tiles = interactive_objects[i]->get_actual_tiles();
				for (auto& til : act_tiles)
				{
					interactive_grid[til].erase(std::remove(interactive_grid[til].begin(), interactive_grid[til].end(), interactive_objects[i].get()), interactive_grid[til].end());
				}
				interactive_objects.erase(interactive_objects.begin() + i);
				i--;
				continue;
			}
	
		}
		if (interactive_objects[i]->get_punched() > 0)
			interactive_objects[i]->update_punched(dt);

		interactive_objects[i]->texture_update(dt);
	}
}

std::vector<std::unique_ptr<Interactive>>& Level::get_interactive_objects()
{
	return interactive_objects;
}

std::vector<std::vector<Interactive*>>& Level::get_interactive_grid()
{
	return interactive_grid;
}

std::vector<std::vector<Character*>>& Level::get_character_grid()
{
	return character_grid;
}

std::vector<bool>& Level::get_collision_array()
{
	return collision_array;
}

const int Level::get_tiles_in_level()&
{
	return tiles_in_level;
}
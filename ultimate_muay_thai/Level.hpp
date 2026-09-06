#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "TileMap.hpp"
#include <array>
#include <vector>
#include "Interactive.hpp"
#include "Player.hpp"
#include "Character.hpp"
#include "Checkpoint.hpp"


class Level :public TileMap
{
protected:
	friend class Game;

	sf::Texture background; //background of the level
	sf::RectangleShape background_shape; //shape of the background of the level
	TileMap tilemap; //tilemap of the level

	std::vector<int> tilemap_array;
	std::vector<bool> collision_array;
	std::vector<std::unique_ptr<Interactive>> interactive_objects;
	std::vector<std::vector<Interactive*>> interactive_grid;
	std::vector<std::vector<Character*>> character_grid;
	sf::View camera; //camera of the level

	short int tiles_in_row; //number of tiles in a row of the level
	short int width;
	short int height;
	int tiles_in_level;
	
public:
	void set_lvl_background(); //setting the background of the level
	TileMap& get_tilemap(); //getting the tilemap of the level
	void load_level(const std::string& path, Player& p1);
	void set_camera_center(sf::Vector2f center_pos);
	void update_interactive_objects(sf::Time& dt, sf::FloatRect& player_hitbox, sf::Vector2f& player_pos, Player& p1, int tiles_in_row);

	//getters
	const int get_tiles_in_row(); //getting the number of tiles in a row of the level
	sf::RectangleShape& get_background(); //getting the background of the level
	std::vector<std::vector<Interactive*>>& get_interactive_grid();
	std::vector<std::unique_ptr<Interactive>>& get_interactive_objects();
	std::vector<bool>& get_collision_array();
	const int get_tiles_in_level()&;
	std::vector<std::vector<Character*>>& get_character_grid();
};
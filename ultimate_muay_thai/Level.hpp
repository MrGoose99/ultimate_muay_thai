#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "TileMap.hpp"
#include <array>
#include <vector>
#include "Interactive.hpp"


class Level :public TileMap
{
protected:
	friend class Game;
	sf::Texture background; //background of the level
	sf::RectangleShape background_shape; //shape of the background of the level
	TileMap tilemap; //tilemap of the level
	std::array<int, 128> interactive_array; //array to determine the interactive objects in the level (0 - no interactive object, 1 - gem hp, 2 - gem special, etc.)
	sf::View camera; //camera of the level
	int tiles_in_row; //number of tiles in a row of the level
public:
	void set_lvl_background(); //setting the background of the level
	virtual void set_lvl_tiles() = 0; //setting the tiles of the level
	sf::RectangleShape& get_background(); //getting the background of the level
	TileMap& get_tilemap(); //getting the tilemap of the level
	virtual std::array<bool,128> get_collision_array() = 0;

	void set_camera_center(sf::Vector2f center_pos);
	virtual void update_interactive_objects(sf::Time& dt) = 0;
	virtual std::vector<std::unique_ptr<Interactive>>& get_interactive_objects() = 0;
	const int get_tiles_in_row(); //getting the number of tiles in a row of the level


};
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "TileMap.hpp"
#include <array>
#include <vector>


class Level :public TileMap
{
protected:
	friend class Game;
	sf::Texture background; //background of the level
	sf::RectangleShape background_shape; //shape of the background of the level
	TileMap tilemap; //tilemap of the level
	sf::View camera; //camera of the level
public:
	void set_lvl_background(); //setting the background of the level
	virtual void set_lvl_tiles() = 0; //setting the tiles of the level
	sf::RectangleShape& get_background(); //getting the background of the level
	TileMap& get_tilemap(); //getting the tilemap of the level
	virtual std::array<bool,128> get_collision_array() = 0;
	void set_camera_center(sf::Vector2f center_pos);

};
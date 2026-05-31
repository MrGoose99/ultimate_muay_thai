#include "Level.hpp"
#include <iostream>
#include "TileMap.hpp"


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
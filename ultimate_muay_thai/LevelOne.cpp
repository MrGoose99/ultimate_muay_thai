#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "LevelOne.hpp"
#include <iostream>
#include <vector>
#include "TileMap.hpp"
#include <array>



LevelOne::LevelOne()
{
	background_shape.setSize({ 1920,1080 });
	background_shape.setTexture(&background);
	background_shape.setPosition({ 0,0 });

	tilemap.set_tilemap("tilesets/tileset_lvl_1.png", { 128,128 }, tilemap_array.data(), 16, 8);


	camera.setSize({ 1920,1080 });
	

}

void LevelOne::set_lvl_tiles()
{
	

}

std::array<bool, 128> LevelOne::get_collision_array()
{
	return collision_array;
}
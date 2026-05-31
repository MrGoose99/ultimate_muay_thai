#include "LevelTwo.hpp"
#include <SFML/Graphics.hpp>
#include <array>

LevelTwo::LevelTwo()
{
	background_shape.setSize({ 1920,1080 });
	background_shape.setTexture(&background);
	background_shape.setPosition({ 0,0 });
}

void LevelTwo::set_lvl_tiles()
{

}

std::array<bool, 128> LevelTwo::get_collision_array()
{
	return collision_array;
}

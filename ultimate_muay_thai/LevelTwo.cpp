#include "LevelTwo.hpp"
#include <SFML/Graphics.hpp>
#include <array>

LevelTwo::LevelTwo()
{
	background_shape.setSize({ 1920,1080 });
	background_shape.setTexture(&background);
	background_shape.setPosition({ 0,0 });
}
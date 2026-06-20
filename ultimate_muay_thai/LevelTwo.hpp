#pragma once

#include "Level.hpp"
#include <SFML/Graphics.hpp>
#include <array>

class LevelTwo :public Level
{
protected:
	sf::Texture background{ "textures/level_2_background.png" }; //background of the level

public:
	LevelTwo();

};
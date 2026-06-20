#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Level.hpp"
#include "Game.hpp"
#include <vector>
#include "TileMap.hpp"
#include <array>


class LevelOne : public Level
{
protected:
	sf::Texture background{ "textures/level_1_background.png" }; //background of the level
	sf::Texture tileset{ "tilesets/tileset_lvl_1.png" }; //tileset of level

public:
	LevelOne();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;



};


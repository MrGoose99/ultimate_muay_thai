#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Level.hpp"
#include "Game.hpp"
#include <vector>
#include "TileMap.hpp"
#include <array>
#include "Player.hpp"
#include "Character.hpp"


class LevelOne : public Level
{
protected:
	sf::Texture background{ "textures/level_1_background_new.png" }; //background of the level
	sf::Texture tileset{ "tilesets/tileset_lvl_1.png" }; //tileset of level

public:
	LevelOne(Player& p1);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;



};


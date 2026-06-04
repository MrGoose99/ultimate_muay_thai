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
    inline static constexpr std::array<int,128>  tilemap_array = { //tilemapa
        0,  0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0,  0, 0, 0, 10, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0,  0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2,
        12, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 4, 5, 5,
        5,  3, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 8,
        5,  5, 2, 2, 2,  3, 0, 1, 2, 2, 2, 3, 0, 0, 0, 0,
        5,  5, 5, 5, 5,  6, 0, 4, 5, 5, 5, 6, 0, 0, 0, 0,
        5,  5, 5, 5, 5,  6, 0, 4, 5, 5, 5, 6, 0, 1, 2, 3
    };
	inline static std::array<bool, 128> collision_array; //collision map (1 - solid tile, 0 - non-solid tile)
    
public:
	LevelOne();
	void set_lvl_tiles() override; //setting the tiles of the level
	void check_camera_events(); //checking for events related to the camera
    std::array<bool, 128> get_collision_array() override;
};


#pragma once

#include "Level.hpp"
#include <SFML/Graphics.hpp>
#include <array>

class LevelTwo :public Level
{
protected:
	sf::Texture background{ "textures/level_2_background.png" }; //background of the level
    inline static constexpr std::array<bool, 128> collision_array = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1
    };

	inline static std::array<int, 128>  interactive_array =
	{
		0,  0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,  0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,  0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,  0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,  0, 1, 1, 1,  0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0,
		0,  0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,  0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,  0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	inline static std::vector<std::unique_ptr<Interactive>> interactive_objects;
public:
	LevelTwo();
	void set_lvl_tiles() override; //setting the tiles of the level
	std::array<bool, 128> get_collision_array() override;
	void update_interactive_objects(sf::Time& dt) override; //updating the interactive objects of the level
    std::vector<std::unique_ptr<Interactive>>& get_interactive_objects() override;
};
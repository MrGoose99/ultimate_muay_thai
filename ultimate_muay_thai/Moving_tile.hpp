#pragma once
#include <SFML/Graphics.hpp>
#include "Interactive.hpp"

class Moving_tile : public Interactive
{
protected:
	sf::Texture object_texture{ "textures/moving_tile.png" };
	sf::Sprite moving_tile_sprite{ object_texture };
	bool status = { 1 };
	short int starting_tile;
	short int nr_of_tiles;
	short int direction;
	sf::Vector2f starting_position;
	sf::Vector2f ending_position;

	sf::Time stop_time = { sf::seconds(0.f) };

	std::string type = { "moving_tile" };

public:
	Moving_tile(short int starting, short int tiles, short int dir, short int tiles_in_row);
	void update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, sf::FloatRect& player_hitbox, sf::Vector2f& player_pos) override;
	void texture_update(sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//getters
	std::string get_object_type() override;
	const sf::Sprite& get_object_sprite() const override;
	
	
};
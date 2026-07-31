#pragma once
#include <SFML/Graphics.hpp>
#include "Interactive.hpp"

class Bullet : public Interactive
{
protected:
	sf::Texture bullet_texture{ "textures/bullet.png" };
	sf::Sprite bullet_sprite{ bullet_texture };

	std::string object_type = { "bullet" };

	bool direction = { 0 }; //0 - left, 1 - right
	int starting_tile;
	sf::Vector2f starting_position;

public:
	Bullet(int starting, bool dir, int tiles_in_row, sf::Vector2f& start_pos);
	void update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid) override;
	void texture_update(sf::Time& dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//setters


	//getters
	std::string get_object_type() override;
	const sf::Sprite& get_object_sprite() const override;
};

#pragma once
#include <SFML/Graphics.hpp>
#include "Interactive.hpp"

class Punching_bag : public Interactive
{
protected:
	sf::Texture object_texture{ "textures/punching_bag.png" };
	sf::Sprite punching_bag_sprite{ object_texture };
	std::string type = "punching_bag";
	short int hp;
	sf::IntRect default_texture_rect;
public:
	Punching_bag(short int tile_nr);
	void texture_update(sf::Time& dt) override;
	void update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, sf::FloatRect& player_hitbox, sf::Vector2f& player_pos) override; //it's not moving, so its empty method
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	//getters
	std::string get_object_type() override; //getting the type of the interactive object (for example, "gem", "heart", etc.)
	const sf::Sprite& get_object_sprite() const override; //getting the sprite of the interactive object

};
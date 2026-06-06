#pragma once
#include <SFML/Graphics.hpp>
#include "Interactive.hpp"

class Spiked_roller : public Interactive
{
protected:
	sf::Texture object_texture{ "textures/spiked_roller.png" };
	sf::Sprite spiked_roller_sprite{ object_texture };
	std::string type = "spiked_roller";
	sf::Time time_animation = {sf::seconds(0.f)};
	sf::IntRect default_texture_rect;
	int animation_stage;
public:
	Spiked_roller(short int tile_nr);
	void texture_update(sf::Time& dt) override;
	void update(sf::Time& dt, float moving_speed) override; //it's not moving, so its empty method
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	//getters
	std::string get_object_type() override; //getting the type of the interactive object (for example, "gem", "heart", etc.)
	sf::Sprite get_object_sprite() override; //getting the sprite of the interactive object
};
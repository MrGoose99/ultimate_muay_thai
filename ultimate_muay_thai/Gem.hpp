#pragma once
#include <SFML/Graphics.hpp>
#include "Interactive.hpp"

class Gem : public Interactive
{
protected:
	sf::Texture object_texture{ "textures/red_gem.png" };
	sf::Sprite gem_sprite{ object_texture };
	short int gem_status; //0 - red (hp), 1 - white (special)
	const float max_y_difference = { 10.f }; //maximum difference between the original position and the current position of the gem (for moving up and down)
	bool go_down = { 1 };
	sf::Vector2f original_position; //original position of the gem (for moving up and down)
	std::string type = "gem"; //type of the interactive object (for example, "gem", "heart", etc.)

public:
	Gem(short int gem_stat = 0, short int tile_nr = 0, bool stat = 1);

	void texture_update(sf::Time& dt) override; //gem havn't animation, so its empty method

	void update(sf::Time& dt, float moving_speed); //update the status of the gem (postition, interacticve)

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override; //drawing the gem

	//getters
	std::string get_object_type() override; //getting the type of the interactive object (for example, "gem", "heart", etc.)
	sf::Sprite get_object_sprite() override; //getting the sprite of the interactive object

};
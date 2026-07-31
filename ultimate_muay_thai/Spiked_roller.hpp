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
	Spiked_roller(int tile_nr, int tiles_in_row);
	void texture_update(sf::Time& dt) override;
	void update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid) override; //it's not moving, so its empty method
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	//getters
	std::string get_object_type() override; //getting the type of the interactive object (for example, "gem", "heart", etc.)
	const sf::Sprite& get_object_sprite() const override; //getting the sprite of the interactive object
};
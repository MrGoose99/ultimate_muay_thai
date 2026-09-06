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
	const short int MAX_HP = 5;

public:
	Punching_bag(int tile_nr, int tiles_in_row);
	void set_hp_to_max();
	void set_position(sf::Vector2f position);
	void set_scale(sf::Vector2f scale);
	void texture_update(sf::Time& dt) override;
	void update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid) override; //it's not moving, so its empty method
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	//getters
	std::string get_object_type() override; //getting the type of the interactive object (for example, "gem", "heart", etc.)
	const sf::Sprite& get_object_sprite() const override; //getting the sprite of the interactive object

};
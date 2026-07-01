#pragma once
#include <SFML/Graphics.hpp>
#include "Interactive.hpp"

class Spikes : public Interactive
{
protected:
	sf::Texture object_texture{ "textures/spikes.png" };
	sf::Sprite spikes_sprite{ object_texture };
	std::string type = "spikes";

	//DIRECTION: 1-ceiling, 2-floor, 3-left_wall, 4-right_wall

public:
	Spikes(int& tile, int dir, int tiles_in_row);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level);
	void texture_update(sf::Time& dt);
	virtual std::string get_object_type(); //getting the type of the interactive object (for example, "gem", "heart", etc.)
	virtual const sf::Sprite& get_object_sprite() const; //getting the sprite of the interactive object
};
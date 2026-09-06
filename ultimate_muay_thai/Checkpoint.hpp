#pragma once
#include "Interactive.hpp"
#include <SFML/Graphics.hpp>

class Checkpoint : public Interactive
{
protected:
	std::string type = { "checkpoint" };
	sf::Texture tex{ "textures/red_gem.png" };
	sf::Sprite checkpoint_sprite{ tex };
	sf::Font font{ "fonts/Pixeled.ttf" };
	sf::Text checkpoint_text{ font };
	sf::Text checkpoint_text_shadow{ font };
	sf::Time drawing_time = { sf::seconds(0.f) };
public:
	Checkpoint(float x, float y, int tile, int tiles_in_row);
	void update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid) override;
	void texture_update(sf::Time& dt) override;
	std::string get_object_type() override;
	const sf::Sprite& get_object_sprite() const override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
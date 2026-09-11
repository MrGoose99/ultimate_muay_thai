#pragma once
#include <SFML/Graphics.hpp>
#include "Interactive.hpp"

class You_win : public Interactive
{
protected:
	sf::FloatRect you_win_rect;
	sf::Texture you_win_texture{ "textures/red_gem.png" };
	sf::Sprite you_win_sprite{ you_win_texture };
	std::string type = { "you_win" }; 
public:
	You_win(float x, float y, int tile, int tiles_in_row);
	void update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid) override;
	void texture_update(sf::Time& dt) override;
	std::string get_object_type() override;
	const sf::Sprite& get_object_sprite() const override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
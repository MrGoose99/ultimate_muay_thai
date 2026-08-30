#pragma once
#include "Character.hpp"
#include <SFML/Graphics.hpp>

class Menu_fighter : public Character
{
protected:
public:
	Menu_fighter(sf::Vector2f position, sf::Vector2f scale);
	void update_character_animation(sf::Time& dt) override; //setting the move animation of the character
	void update_frame_status(sf::Time& dt) override; //updating the status of the frame (for example, for a 2-frame animation, it will be 0 for the first frame and 1 for the second frame)
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
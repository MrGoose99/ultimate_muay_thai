#pragma once
#include <SFML/Graphics.hpp>

class Interactive: public sf::Drawable, public sf::Transformable
{
protected:
	sf::FloatRect hitbox;
	sf::Texture object_texture;
	bool status;

public:
	void virtual update(sf::Time& dt, float moving_speed) = 0; //updating the status of the interactive object (position, interactive)
	void virtual texture_update(sf::Time& dt) = 0;

	//getters
	virtual std::string get_object_type() = 0; //getting the type of the interactive object (for example, "gem", "heart", etc.)
	virtual sf::Sprite get_object_sprite() = 0; //getting the sprite of the interactive object

	//setters
	void set_status(bool stat);

	//getters
	bool get_status();
};
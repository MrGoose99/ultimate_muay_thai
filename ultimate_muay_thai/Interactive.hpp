#pragma once
#include <SFML/Graphics.hpp>

class Interactive: public sf::Drawable, public sf::Transformable
{
protected:
	sf::FloatRect hitbox;
	sf::Texture object_texture;
	bool status = { 1 };
	int hp = { 5 };
	int tile_number = { 0 };
	short int punched = { 0 };
	sf::Time punched_time = { sf::seconds(0.f) };

public:
	void virtual update(sf::Time& dt, float moving_speed) = 0; //updating the status of the interactive object (position, interactive)
	void virtual texture_update(sf::Time& dt) = 0;
	void update_punched(sf::Time& dt);

	//getters
	virtual std::string get_object_type() = 0; //getting the type of the interactive object (for example, "gem", "heart", etc.)
	virtual sf::Sprite get_object_sprite() = 0; //getting the sprite of the interactive object
	bool get_status();
	int get_hp()&;
	const short int get_tile_number()&;
	const short int get_punched()&;

	//setters
	void set_status(bool stat);
	void decrease_hp(short int points);
	void set_punched(short int p);


};
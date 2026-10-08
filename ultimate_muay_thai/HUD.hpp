#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.hpp"

class HUD : public sf::Drawable, public sf::Transformable
{
private:
	//visuals//////////////////////////////////////////////////////////////////////////////////////
	sf::Texture hud_texture{ "textures/HUD.png" }; //texture of the HUD
	sf::Sprite hud_shape{ hud_texture }; //shape of the HUD
	sf::Texture head_texture{ "textures/head.png" };
	sf::Sprite heads[3] =
	{
		sf::Sprite{head_texture}, sf::Sprite{head_texture}, sf::Sprite{head_texture}
	};
	sf::RectangleShape hp_bar; //health points bar
	sf::RectangleShape special_bar; //special bar

	sf::Texture controls_texture{ "textures/controls.png" };
	sf::Sprite controls_sprite{ controls_texture };

	sf::Texture o_button_texture{ "textures/o_button.png" };
	sf::Sprite o_button_sprite{ o_button_texture };

	//MAX SIZE
	const float special_bar_max_size_x = 203.f;

	float change = { 0.f };

	//bool
	bool up_down = false;
	bool controls_active = { false };

public:
	//constructor//////////////////////////////////////////////////////////////////////////////////////
	HUD();

	//update////////////////////////////////////////////////////////////////////////////////////////
	void hud_update(short int hp, short int max_hp, short int spec, short int max_spec, const bool pistol_mode, sf::Time& dt, short int lifes);

	//drawing//////////////////////////////////////////////////////////////////////////////////////
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override; //drawing the HUD

	//getters//////////////////////////////////////////////////////////////////////////////////////
	bool get_controls_active() const; //returning the status of the controls
	//setters//////////////////////////////////////////////////////////////////////////////////////
	void set_controls_active(const bool status); //setting the status of the controls
};
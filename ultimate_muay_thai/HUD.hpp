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
	sf::RectangleShape hp_bar; //health points bar
	sf::RectangleShape special_bar; //special bar

	//MAX SIZE
	const float special_bar_max_size_x = 203.f;

public:
	//constructor//////////////////////////////////////////////////////////////////////////////////////
	HUD();

	//update////////////////////////////////////////////////////////////////////////////////////////
	void hud_update(short int hp, short int max_hp, short int spec, short int max_spec);

	//drawing//////////////////////////////////////////////////////////////////////////////////////
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override; //drawing the HUD

};
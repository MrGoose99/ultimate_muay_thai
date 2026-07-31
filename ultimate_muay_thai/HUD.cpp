#include <SFML/Graphics.hpp>
#include "HUD.hpp"
#include "Player.hpp"
#include <iostream>

HUD::HUD()
{
	//HUD STATIC
	if (!hud_texture.loadFromFile("textures/HUD.png"))
	{
		std::cout << "Error loading HUD texture from file\n";
	}
	hud_shape.setTexture(hud_texture);
	hud_shape.setPosition({ 0.f,0.f });
	//HP
	hp_bar.setPosition({ 258.f, 97.f });
	hp_bar.setSize({ 327.f, 18.f });
	hp_bar.setFillColor(sf::Color::Red);
	//SPECIAL
	special_bar.setPosition({ 258.f, 128.f });
	special_bar.setSize({ 0.f,7.f });
	special_bar.setFillColor(sf::Color(215, 211, 146));

}

void HUD::hud_update(short int hp, short int max_hp, short int spec, short int max_spec, const bool pistol_mode, sf::Time& dt)
{
	hp_bar.setSize({ 327.f *  hp / max_hp, 18.f });
	special_bar.setSize({ special_bar_max_size_x * spec / max_spec, 7.f });
	sf::Color color = special_bar.getFillColor();

	if (pistol_mode)
	{
		change += 255.f * dt.asSeconds();
		if (color.a <= 0.f) up_down = 1;
		else if (color.a >= 255.f) up_down = 0;

		if (change >= 1.f)
		{
			if (up_down)
			{
				color.a += change;
			}
			else
			{
				color.a -= change;
			}
			change = 0.f;
		}
		special_bar.setFillColor(color);
	}
	else
	{
		special_bar.setFillColor(sf::Color(215, 211, 146, 255));
		change = 0.f;
		up_down = 0;
	}
}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(hp_bar, states);
	target.draw(special_bar, states);
	target.draw(hud_shape, states);
}
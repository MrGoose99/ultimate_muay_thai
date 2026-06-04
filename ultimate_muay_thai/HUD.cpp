#include <SFML/Graphics.hpp>
#include "HUD.hpp"
#include "Player.hpp"

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

void HUD::hud_update(short int hp, short int max_hp, short int spec, short int max_spec)
{
	hp_bar.setSize({ 327.f *  hp / max_hp, 18.f });
	special_bar.setSize({ special_bar_max_size_x * spec / max_spec, 7.f });
}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(hp_bar, states);
	target.draw(special_bar, states);
	target.draw(hud_shape, states);
}
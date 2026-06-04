#include <SFML/Graphics.hpp>
#include "HUD.hpp"

HUD::HUD()
{
	
	if (!hud_texture.loadFromFile("textures/HUD.png"))
	{
		std::cout << "Error loading HUD texture from file\n";
	}
		hud_shape.setTexture(hud_texture);
		hud_shape.setPosition({ 0.f,0.f });
}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(hud_shape, states);
}
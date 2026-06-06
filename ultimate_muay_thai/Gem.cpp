#include "Gem.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>

Gem::Gem(short int gem_stat, short int tile_nr, bool stat)
{
	gem_status = gem_stat;
	status = stat;
	if (gem_status == 0)
	{
		if (!object_texture.loadFromFile("textures/red_gem.png"))
			std::cout << "Error loading heart texture from file\n";
		type = "hp_gem";
	}
	else if (gem_status == 1)
	{
		if (!object_texture.loadFromFile("textures/white_gem.png"))
			std::cout << "Error loading special texture from file\n";
		type = "special_gem";
	}
	gem_sprite.setTexture(object_texture);

	float position_x = tile_nr % 16 * 128.f + 48.f;
	float position_y = tile_nr / 16 * 128.f + (rand() % int(max_y_difference) + 56.f);
	original_position = { position_x, tile_nr / 16 * 128.f + 56.f };
	if (rand() % 100 < 50) go_down = 1;
	else go_down = 0;
	gem_sprite.setPosition({ position_x, position_y });

	hitbox.position = { position_x, position_y };
	hitbox.size = sf::Vector2f{ object_texture.getSize() };


	
}

void Gem::update(sf::Time& dt, float moving_speed)
{
	if (status)
	{
		if (go_down)
		{
			gem_sprite.move({ 0.f, moving_speed * dt.asSeconds() });
			if (gem_sprite.getPosition().y - original_position.y >= max_y_difference)
				go_down = 0;
		}
		else
		{
			gem_sprite.move({ 0.f, -moving_speed * dt.asSeconds() });
			if (original_position.y - gem_sprite.getPosition().y >= max_y_difference)
				go_down = 1;
		}
	}


}

void Gem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(status) target.draw(gem_sprite, states);
}

std::string Gem::get_object_type()
{
	return type;
}

sf::Sprite Gem::get_object_sprite()
{
	return gem_sprite;
}

void Gem::texture_update(sf::Time& dt)
{

}
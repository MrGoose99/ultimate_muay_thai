#include "Punching_bag.hpp"
#include "Interactive.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Punching_bag::Punching_bag(short int tile_nr)
{
	tile_number = tile_nr;
	if (!object_texture.loadFromFile("textures/punching_bag.png"))
		std::cout << "Error loading punching_bag texture...\n";
	
	punching_bag_sprite.setTexture(object_texture);
	default_texture_rect; default_texture_rect.position = { 0,0 }; default_texture_rect.size = { 54, 123 };
	punching_bag_sprite.setTextureRect(default_texture_rect);

	float position_x = tile_nr % 16 * 128.f + 37.f;
	float position_y = tile_nr / 16 * 128.f;

	punching_bag_sprite.setPosition({ position_x, position_y });

	status = 1;

	set_punched(0);

	hp = 5;
}

void Punching_bag::texture_update(sf::Time& dt)
{
	sf::IntRect new_tex_rect;
	if (get_punched() == 1)
	{
		 new_tex_rect.position = { default_texture_rect.size.x, 0}; new_tex_rect.size = {60, 123};
	}
	else if (get_punched() == 2)
	{
		new_tex_rect.position = { default_texture_rect.size.x + 60,0 }; new_tex_rect.size = { 60, 123 };
	}
	else
	{
		new_tex_rect = default_texture_rect;
	}
	punching_bag_sprite.setTextureRect(new_tex_rect);
	

}

void Punching_bag::update(sf::Time& dt, float moving_speed)
{
	//empty method
}

void Punching_bag::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(punching_bag_sprite, states);
}

std::string Punching_bag::get_object_type()
{
	return type;
}

sf::Sprite Punching_bag::get_object_sprite()
{
	return punching_bag_sprite;
}

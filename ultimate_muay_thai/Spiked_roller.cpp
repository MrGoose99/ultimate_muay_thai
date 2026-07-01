#include "Interactive.hpp"
#include "Spiked_roller.hpp"
#include <iostream>
#include "Player.hpp"

Spiked_roller::Spiked_roller(int tile_nr, int tiles_in_row)
{
	tile_number = tile_nr;
	spawning_tile = tile_nr;
	current_tile = tile_nr;
	if(!object_texture.loadFromFile("textures/spiked_roller.png"))
		std::cout << "Error spiked roller texture loading...\n";
	spiked_roller_sprite.setTexture(object_texture);


	default_texture_rect.position = { 0, 0 };
	default_texture_rect.size = { 78, 120 };
	spiked_roller_sprite.setTextureRect(default_texture_rect);

	float position_x = tile_nr % tiles_in_row * 128.f + 25.f;
	float position_y = tile_nr / tiles_in_row * 128.f + 5.f;

	spiked_roller_sprite.setPosition({ position_x, position_y });

	status = 1;

	animation_stage = 0;

}

void Spiked_roller::texture_update(sf::Time& dt)
{
	time_animation += dt;

	if (time_animation >= sf::seconds(0.2f))
	{
		time_animation = sf::seconds(0.f);
		if (animation_stage == 0)
		{
			animation_stage = 1;
			sf::IntRect changing;
			changing.position = { default_texture_rect.position.x + 78, default_texture_rect.position.y };
			changing.size = default_texture_rect.size;
			spiked_roller_sprite.setTextureRect(changing);
		}

		else
		{
			animation_stage = 0;
			spiked_roller_sprite.setTextureRect(default_texture_rect);
		}
	}
}

void Spiked_roller::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (status) target.draw(spiked_roller_sprite, states);
}

void Spiked_roller::update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level)
{

}

std::string Spiked_roller::get_object_type()
{
	return type;
}

const sf::Sprite& Spiked_roller::get_object_sprite() const
{
	return spiked_roller_sprite;
}
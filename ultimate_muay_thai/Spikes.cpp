#include "Spikes.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.hpp"

Spikes::Spikes(int& tile, int dir, int tiles_in_row)
{
	tile_number = tile;
	current_tile = tile;
	direction = dir;
	status = 1;
	if (!object_texture.loadFromFile("textures/spikes.png"))
		std::cout << "Error loading spikes texture...";

	spikes_sprite.setTexture(object_texture);
	
	float position_x = (tile % tiles_in_row) * 128.f;
	float position_y = (tile / tiles_in_row) * 128.f;

	spikes_sprite.setPosition({ position_x, position_y });
	spikes_sprite.setOrigin({ 64.f, 0.f });
	switch (direction)
	{
	case 1:
		spikes_sprite.setRotation(sf::Angle::Zero);
		spikes_sprite.move({ 64.f, 0.f });
		break;
	case 2:
		spikes_sprite.setRotation(sf::degrees(180));
		spikes_sprite.move({ 64.f, 128.f });
		break;
	case 3:
		spikes_sprite.setRotation(sf::degrees(270));
		spikes_sprite.move({ 0.f, 64.f });
		break;
	case 4:
		spikes_sprite.setRotation(sf::degrees(90));
		spikes_sprite.move({ 128.f, 64.f });
		break;
	default:
		spikes_sprite.setRotation(sf::Angle::Zero);
	}
	
}

void Spikes::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(spikes_sprite, states);
}

void Spikes::update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level)
{
	//empty virtual
}

void Spikes::texture_update(sf::Time& dt)
{
	//empty vitual
}

std::string Spikes::get_object_type()
{
	return type;
}

const sf::Sprite& Spikes::get_object_sprite() const
{
	return spikes_sprite;
}
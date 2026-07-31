#include <SFML/Graphics.hpp>
#include "Interactive.hpp"
#include "Moving_tile.hpp"
#include <iostream>
#include "Player.hpp"

Moving_tile::Moving_tile(short int starting, short int tiles, short int dir, short int tiles_in_row)
{
	spawning_tile = starting;
	nr_of_tiles = tiles;
	current_tile = starting;
	direction = dir; //1 - right, 2 - left, 3 - up, 4 - down

	if (!object_texture.loadFromFile("textures/moving_tile.png"))
		std::cout << "Error loading texture of moving tile...";
	moving_tile_sprite.setTexture(object_texture);



	switch (dir) //min and max setting
	{
	case 1:
		starting_position = { starting % tiles_in_row * 128.f, starting / tiles_in_row * 128.f };
		ending_position = { starting_position.x + (tiles * 128.f + 64.f), starting_position.y };
		break;
	case 2:
		starting_position = { starting % tiles_in_row * 128.f + 64.f, starting / tiles_in_row * 128.f };
		ending_position = { (starting_position.x - 64.f) - (tiles * 128.f), starting_position.y };
		break;
	case 3:
		starting_position = { starting % tiles_in_row * 128.f + 32.f, starting / tiles_in_row * 128.f + (128.f - 16.f) };
		ending_position = { starting_position.x, starting_position.y - (tiles * 128.f) - 112.f };
		break;
	case 4:
		starting_position = { starting % tiles_in_row * 128.f + 32.f, starting / tiles_in_row * 128.f };
		ending_position = { starting_position.x, starting_position.y + (tiles * 128.f) + (128.f - 16.f) };
		break;
	default:
		starting_position = { starting % tiles_in_row * 128.f, starting / tiles_in_row * 128.f };
		ending_position = { starting_position.x + (tiles * 128.f) + 64.f, starting_position.y };
	}

	moving_tile_sprite.setPosition(starting_position);
}

std::string Moving_tile::get_object_type()
{
	return type;
}

const sf::Sprite& Moving_tile::get_object_sprite() const
{
	return moving_tile_sprite;
}

void Moving_tile::update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid)
{

	sf::Vector2f new_pos;
	float pos_x = moving_tile_sprite.getPosition().x;
	float pos_y = moving_tile_sprite.getPosition().y;
	switch (direction)
	{
	case 1:
		new_pos = { pos_x + (moving_speed * dt.asSeconds()), pos_y };
		if (new_pos.x >= ending_position.x)
		{
			new_pos = ending_position;
			direction = 2;
			std::swap(starting_position, ending_position);
		}
		actual_velocity_x = moving_speed * dt.asSeconds();
		actual_velocity_y = 0.f;
		break;
	case 2:
		new_pos = { pos_x - (moving_speed * dt.asSeconds()), pos_y };
		if (new_pos.x <= ending_position.x)
		{
			new_pos = ending_position;
			direction = 1;
			std::swap(starting_position, ending_position);
		}
		actual_velocity_x = -moving_speed * dt.asSeconds();
		actual_velocity_y = 0.f;
		break;
	case 3:
		new_pos = { pos_x, pos_y - (moving_speed * dt.asSeconds()) };
		if (new_pos.y <= ending_position.y)
		{
			new_pos = ending_position;
			direction = 4;
			std::swap(starting_position, ending_position);
		}
		actual_velocity_x = 0.f;
		actual_velocity_y = -moving_speed * dt.asSeconds();
		break;
	case 4:
		new_pos = { pos_x, pos_y + (moving_speed * dt.asSeconds()) };
		if (new_pos.y >= ending_position.y)
		{
			new_pos = ending_position;
			direction = 3;
			std::swap(starting_position, ending_position);
		}
		actual_velocity_x = 0.f;
		actual_velocity_y = moving_speed * dt.asSeconds();
		break;
	default:
		break;
	}

	moving_tile_sprite.setPosition(new_pos);

	for (int i = 0; i < actual_tiles.size(); i++)
	{
		int tile = actual_tiles[i];
		moving_objects[tile].erase(std::remove(moving_objects[tile].begin(), moving_objects[tile].end(), this), moving_objects[tile].end());
	}
	actual_tiles.clear();


	int left = new_pos.x / 128.f;
	int right = (new_pos.x + moving_tile_sprite.getLocalBounds().size.x) / 128.f;
	int top = new_pos.y / 128.f;
	int bottom = (new_pos.y + moving_tile_sprite.getLocalBounds().size.y) / 128.f;



	for(int y = top; y <= bottom; y++)
		for (int x = left; x <= right; x++)
		{

			if (x < 0 || y < 0 || x >= tiles_in_row || y >= moving_objects.size() / tiles_in_row)
			{

				continue;
			}
			else
			{

				int checking_tile = x + y * tiles_in_row;
				bool is_there = 0;
				for (auto& tiles : actual_tiles)
					if (tiles == checking_tile) is_there = 1;
				if (!is_there)
				{
					actual_tiles.push_back(checking_tile);
					moving_objects[checking_tile].push_back(this);
				}
			}
		}

}

void Moving_tile::texture_update(sf::Time& dt)
{
	//empty
}

void Moving_tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(moving_tile_sprite, states);
}

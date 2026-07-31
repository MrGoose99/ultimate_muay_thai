#include <SFML/Graphics.hpp>
#include "Bullet.hpp"
#include "Interactive.hpp"
#include <iostream>

Bullet::Bullet(int starting, bool dir, int tiles_in_row, sf::Vector2f& start_pos)
{
	if (!bullet_texture.loadFromFile("textures/bullet.png"))
		std::cout << "Error loading bullet texture...";
	bullet_sprite.setTexture(bullet_texture);

	starting_tile = starting;
	actual_tiles.push_back(starting);
	direction = dir;
	starting_position = start_pos;

}

void Bullet::update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid)
{
	sf::Vector2f new_pos;
	float pos_x = bullet_sprite.getPosition().x;
	float pos_y = bullet_sprite.getPosition().y;

	if (Bullet::direction)
		pos_x += moving_speed * dt.asSeconds();
	else
		pos_x -= moving_speed * dt.asSeconds();
	
	new_pos = { pos_x, pos_y };


	for (int i = 0; i < actual_tiles.size(); i++)
	{
		int tile = actual_tiles[i];
		moving_objects[tile].erase(std::remove(moving_objects[tile].begin(), moving_objects[tile].end(), this), moving_objects[tile].end());
	}
	actual_tiles.clear();


	int left = new_pos.x / 128.f;
	int right = (new_pos.x + bullet_sprite.getLocalBounds().size.x) / 128.f;
	int top = new_pos.y / 128.f;
	int bottom = (new_pos.y + bullet_sprite.getLocalBounds().size.y) / 128.f;


	for (int y = top; y <= bottom; y++)
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
	std::cout << "BULLET EXIST\n";

}

void Bullet::texture_update(sf::Time& dt)
{
	//empty method
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(bullet_sprite, states);
}

std::string Bullet::get_object_type()
{
	return object_type;
}

const sf::Sprite& Bullet::get_object_sprite() const
{
	return bullet_sprite;
}
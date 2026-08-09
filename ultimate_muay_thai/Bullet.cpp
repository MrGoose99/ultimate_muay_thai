#include <SFML/Graphics.hpp>
#include "Bullet.hpp"
#include "Interactive.hpp"
#include "Character.hpp"
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
	bullet_sprite.setPosition(starting_position);
	status = 1;

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

	sf::FloatRect checking_rect = { new_pos, bullet_sprite.getLocalBounds().size };

	int left = checking_rect.position.x / 128.f;
	int right = (checking_rect.position.x + checking_rect.size.x) / 128.f;
	int top = checking_rect.position.y / 128.f;
	int bottom = (checking_rect.position.y + checking_rect.size.y) / 128.f;
	for (int y = top; y <= bottom; y++)
		for (int x = left; x <= right; x++)
		{
			if (x < 0 || y < 0 || x >= tiles_in_row || y >= tiles_in_level / tiles_in_row)
			{
				continue;
			}
			else if (collision_array[x + y * tiles_in_row])
			{
				sf::FloatRect checking_tile = { {x * 128.f, y * 128.f}, {128,128} };
				if (checking_tile.findIntersection(checking_rect))
				{
					this->set_destroyed(1);
					return;
				}
			}
		}
	for (int y = top; y <= bottom; y++)
		for (int x = left; x <= right; x++)
		{
			if (x < 0 || y < 0 || x >= tiles_in_row || y >= tiles_in_level / tiles_in_row)
			{
				continue;
			}
			else
			{
				short int index = x + y * tiles_in_row;
				for (short int i = 0; i < moving_objects[index].size(); i++)
				{
					if (moving_objects[index][i]->get_object_type() == "spiked_roller"
						|| moving_objects[index][i]->get_object_type() == "spikes"
						|| moving_objects[index][i]->get_object_type() == "punching_bag")
					{
						sf::FloatRect checking_tile = { moving_objects[index][i]->get_object_sprite().getGlobalBounds() };
						if (checking_tile.findIntersection(checking_rect))
						{
							this->set_destroyed(1);
							return;
						}
					}
					else if (moving_objects[index][i]->get_object_type() == "enemy" && !moving_objects[index][i]->get_is_dying())
					{
						sf::FloatRect checking_tile = { moving_objects[index][i]->get_object_sprite().getGlobalBounds() };
						if (checking_tile.findIntersection(checking_rect))
						{
							moving_objects[index][i]->decrease_hp(1000);
							if (moving_objects[index][i]->get_hp() <= 0)
								moving_objects[index][i]->set_by_bullet(true);
								moving_objects[index][i]->set_is_dying(true);
							this->set_destroyed(1);
							return;
						}
					}

				}
			}

			bullet_sprite.setPosition(new_pos);
			for (int i = 0; i < actual_tiles.size(); i++)
			{
				int tile = actual_tiles[i];
				moving_objects[tile].erase(std::remove(moving_objects[tile].begin(), moving_objects[tile].end(), this), moving_objects[tile].end());
			}
			actual_tiles.clear();

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
			if (abs(bullet_sprite.getPosition().x - starting_position.x) >= 1000) this->set_destroyed(1);
		}
}

void Bullet::texture_update(sf::Time& dt)
{
	//empty method
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(bullet_sprite, states);
	//sf::RectangleShape debug_bullet; debug_bullet.setSize({ 10.f, 10.f });
	//debug_bullet.setPosition(bullet_sprite.getPosition()); debug_bullet.setFillColor(sf::Color::Red);
	//target.draw(debug_bullet, states);
}

std::string Bullet::get_object_type()
{
	return object_type;
}

const sf::Sprite& Bullet::get_object_sprite() const
{
	return bullet_sprite;
}
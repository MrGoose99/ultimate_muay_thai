#include "Character.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Interactive.hpp"


void Character::set_character(std::filesystem::path& texture, std::string& char_name, sf::Vector2f& pos)
{
	character_name = char_name;
	if (!character_texture.loadFromFile(texture))  //texture loading
		std::cout << "Error loading character " << character_name << " texture from file\n";
	character_sprite.setTexture(character_texture); //setting the texture to the sprite
	character_sprite.setPosition(pos); //setting the position of the sprite
}

sf::Sprite& Character::get_character_sprite()
{
	return character_sprite;
}

sf::IntRect Character::get_frame_position(short int frame_number)
{
	
	return sf::IntRect(sf::Vector2i(frame_number % (character_texture.getSize().x / 128) * 128, frame_number / (character_texture.getSize().x / 128) * 128), sf::Vector2i(128,128));
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform(); 

	states.texture = &character_texture;

	target.draw(character_sprite, states); 
}

void Character::character_moving(sf::Time& dt, const float speed_normal, const float speed_fight, std::vector<bool>& collision_array, std::vector<std::vector<Interactive*>>& interactive_grid, short int tiles_in_row)
{
	if (!is_fighting)
	{
		if (moving_normal == 2)
		{
			if (right_side == 0)
			{
				right_side = 1;
			}
			sf::Vector2f next_pos = { character_position.x + speed_normal * dt.asSeconds(), character_position.y };
			if (!check_character_collision(next_pos, tiles_in_row, collision_array, interactive_grid))
				character_position = next_pos;
		}
		else if (moving_normal == 1)
		{
			if (right_side == 1)
			{
				right_side = 0;
			}
			sf::Vector2f next_pos = { character_position.x + -speed_normal * dt.asSeconds(), character_position.y };
			if(!check_character_collision(next_pos, tiles_in_row, collision_array, interactive_grid))
				character_position = next_pos;
		}
	}
	else if (is_fighting && !is_falling && !is_jumping)
	{
		if (moving_fight == 2)
		{
			sf::Vector2f next_pos = {character_position.x + speed_fight * dt.asSeconds(), character_position.y };
			if (!check_character_collision(next_pos, tiles_in_row, collision_array, interactive_grid))
			character_position = next_pos;
		}
		else if (moving_fight == 1)
		{
			sf::Vector2f next_pos = { character_position.x + -speed_fight * dt.asSeconds(), character_position.y };
			if (!check_character_collision(next_pos, tiles_in_row, collision_array, interactive_grid))
			character_position = next_pos;
		}
	}
}

const unsigned int Character::get_tile_number(unsigned int tiles_in_row)
{
	return character_position.x / 128 + character_position.y / 128 * tiles_in_row;
}

void Character::character_position_update()
{
	character_sprite.setPosition(character_position);
	hitbox.position = { character_position.x + character_sprite.getLocalBounds().size.x / 2 - hitbox.size.x / 2, character_position.y + character_sprite.getLocalBounds().size.y / 8 };
}

sf::RectangleShape Character::get_debug_shape()
{
	return debug;
}

sf::RectangleShape Character::get_debug_2_shape()
{
	return debug2;
}

bool Character::check_character_collision(const sf::Vector2f position, const int tiles_in_row, std::vector<bool>& collision_array, std::vector<std::vector<Interactive*>>& interactive_grid)
{

	//debug rects
	debug.setSize(hitbox.size);
	debug.setPosition(hitbox.position);
	debug.setFillColor(sf::Color::Red);
	debug2.setSize(attackbox.size);
	debug2.setPosition(attackbox.position);
	if (attackbox_active) debug2.setFillColor(sf::Color::Red);
	else debug2.setFillColor(sf::Color::Green);
	////////////////

	sf::FloatRect checking_rect = { {position.x + character_sprite.getLocalBounds().size.x / 2 - hitbox.size.x / 2, position.y + character_sprite.getLocalBounds().size.y / 8}, hitbox.size};
	int left = checking_rect.position.x / 128.f;
	int right = (checking_rect.position.x + hitbox.size.x) / 128.f;
	int top = checking_rect.position.y / 128.f;
	int bottom = (checking_rect.position.y + hitbox.size.y) / 128.f;
	for(int y = top; y <= bottom; y++)
		for (int x = left; x <= right; x++)
		{
			if (x < 0 || y < 0 || x >= tiles_in_row)
			{
				continue;
			}
			else if (collision_array[x + y * tiles_in_row])
			{
				sf::FloatRect checking_tile = { {x * 128.f, y * 128.f}, {128,128} };
				if (checking_tile.findIntersection(checking_rect))
					return 1;
			}
		}
	for (int y = top; y <= bottom; y++)
		for (int x = left; x <= right; x++)
		{
			if (x < 0 || y < 0 || x >= tiles_in_row)
			{
				continue;
			}
			else
			{
				short int index = x + y * tiles_in_row;
				for (short int i = 0; i < interactive_grid[index].size(); i++)
				{
					if (interactive_grid[index][i]->get_object_type() == "punching_bag"
						|| interactive_grid[index][i]->get_object_type() == "moving_tile")
					{
						sf::FloatRect checking_tile = { {interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position}, {interactive_grid[index][i]->get_object_sprite().getGlobalBounds().size } };
						if (checking_tile.findIntersection(checking_rect))
							return 1;
					}
				}
			}
		}	
			
		
	return 0;
}

void Character::check_velocity_y(sf::Time& dt, std::vector<bool>& collision_array, const int tiles_in_row, std::vector<std::vector<Interactive*>>& interactive_grid)
{
	on_ground = 0;
	velocity_y += gravity * dt.asSeconds();
	float change = std::min(velocity_y, max_fall_speed);
	sf::Vector2f next_pos = { character_position.x, character_position.y + (change * dt.asSeconds()) };
	bool collided = check_character_collision(next_pos, tiles_in_row, collision_array, interactive_grid);
	if (!collided)
	{

		character_position = next_pos;

	}
	else
	{
		if (velocity_y > 0.f)
		{
			on_ground = 1;
			velocity_y = 0.f;
			//float tile_top = next_pos.y + 128.f;
			//tile_top = tile_top / 128.f;
			//tile_top = tile_top * 128.f;

		}
		else
		{
			velocity_y = 0.f;

		}
	}
	is_falling = (!on_ground && velocity_y > 0.f);
	is_jumping = (!on_ground && velocity_y < 0.f);

}

void Character::check_velocity_x(sf::Time& dt, std::vector<bool>& collision_array, std::vector<std::vector<Interactive*>>& interactive_grid)
{
	if (velocity_x < 0.f) velocity_x += gravity * dt.asSeconds();
	else if (velocity_x > 0.f) velocity_x -= gravity * dt.asSeconds();
	float change = velocity_x;
	sf::Vector2f next_pos = { character_position.x + (change * dt.asSeconds()), character_position.y };
	bool collided = check_character_collision(next_pos, 16, collision_array, interactive_grid);
	if (!collided)
	{
		character_position = next_pos;
	}
	else
	{
		velocity_x = 0.f;
	}
	if (on_ground)
		velocity_x = 0.f;
}

void Character::attack(sf::Time& dt)
{
	attack_time += dt;
	if (meele_attack_state != MeeleAttackState::None && kick_attack_state == KickAttackState::None)
	{
		attackbox.size = { hitbox.size.x / 2, hitbox.size.y / 6 };
		if (right_side) attackbox.position = { hitbox.position.x + hitbox.size.x, hitbox.position.y + hitbox.size.y / 7 };
		else attackbox.position = { hitbox.position.x - hitbox.size.x / 2, hitbox.position.y + hitbox.size.y / 7 };
		if (attack_time >= sf::seconds(0.15f) && attack_time <= sf::seconds(0.3f))
		{
			attackbox_active = 1;
		}
		else attackbox_active = 0;
	}
	else attackbox_active = 0;

	if (kick_attack_state != KickAttackState::None && meele_attack_state == MeeleAttackState::None)
	{
		switch (kick_attack_state)
		{
		case KickAttackState::AttackMiddle:
			attackbox.size = { hitbox.size.x / 2, hitbox.size.y / 6 };
			if (right_side)
				attackbox.position = { hitbox.position.x + hitbox.size.x, hitbox.position.y + hitbox.size.y / 2 };
			else
				attackbox.position = { hitbox.position.x - hitbox.size.x / 2, hitbox.position.y + hitbox.size.y / 2 };

			if (attack_time >= sf::seconds(0.15f) && attack_time <= sf::seconds(0.3f))
				attackbox_active = 1;
			else
				attackbox_active = 0;
			break;

		case KickAttackState::AttackHigh:
			attackbox.size = { hitbox.size.x / 2, hitbox.size.y / 6 };
			if (right_side)
				attackbox.position = { hitbox.position.x + hitbox.size.x, hitbox.position.y + hitbox.size.y / 7 };
			else
				attackbox.position = { hitbox.position.x - hitbox.size.x / 2, hitbox.position.y + hitbox.size.y / 7};

			if (attack_time >= sf::seconds(0.25f) && attack_time <= sf::seconds(0.35f))
				attackbox_active = 1;
			else
				attackbox_active = 0;
			break;

		case KickAttackState::AttackLow:
			attackbox.size = { hitbox.size.x / 2, hitbox.size.y / 6 };
			if (right_side)
				attackbox.position = { hitbox.position.x + hitbox.size.x, hitbox.position.y + hitbox.size.y / 6 * 5 };
			else
				attackbox.position = { hitbox.position.x - hitbox.size.x / 2, hitbox.position.y + hitbox.size.y / 6 * 5 };

			if (attack_time <= sf::seconds(0.2f))
				attackbox_active = 1;
			else
				attackbox_active = 0;
			break;

		default:
			attackbox_active = 0;
			break;
		}
	}

	if (attack_time >= sf::seconds(0.45f))
	{
		meele_attack_state = MeeleAttackState::None;
		kick_attack_state = KickAttackState::None;
		attack_time = sf::seconds(0.f);
	}
}

const short int Character::get_hp()
{
	return hp;
}

const short int Character::get_max_hp()
{
	return max_hp;
}

sf::FloatRect& Character::get_character_hitbox()
{
	return hitbox;
}

sf::Vector2f& Character::get_character_position()
{
	return character_position;
}

const short int Character::get_damage()&
{
	if (meele_attack_state != MeeleAttackState::None)
		return 1;
	else if (kick_attack_state == KickAttackState::AttackLow)
		return 1;
	else if (kick_attack_state == KickAttackState::AttackMiddle)
		return 2;
	else if (kick_attack_state == KickAttackState::AttackHigh)
		return 3;
}
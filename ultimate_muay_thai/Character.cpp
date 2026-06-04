#include "Character.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>



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

void Character::character_moving(sf::Time& dt, const float speed_normal, const float speed_fight, std::array<bool, 128> collision_array)
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
			if (!check_character_collision(next_pos, 16, collision_array))
				character_position = next_pos;
		}
		else if (moving_normal == 1)
		{
			if (right_side == 1)
			{
				right_side = 0;
			}
			sf::Vector2f next_pos = { character_position.x + -speed_normal * dt.asSeconds(), character_position.y };
			if(!check_character_collision(next_pos, 16, collision_array))
				character_position = next_pos;
		}
	}
	else if (is_fighting && !is_falling && !is_jumping)
	{
		if (moving_fight == 2)
		{
			sf::Vector2f next_pos = {character_position.x + speed_fight * dt.asSeconds(), character_position.y };
			if (!check_character_collision(next_pos, 16, collision_array))
			character_position = next_pos;
		}
		else if (moving_fight == 1)
		{
			sf::Vector2f next_pos = { character_position.x + -speed_fight * dt.asSeconds(), character_position.y };
			if (!check_character_collision(next_pos, 16, collision_array))
			character_position = next_pos;
		}
	}
}

void Character::character_falling(sf::Time& dt, const float falling_speed, std::array<bool,128> collision_array)
{
	sf::Vector2f next_pos = { character_position.x, character_position.y + dt.asSeconds() * falling_speed };
	if (!check_character_collision(next_pos, 16, collision_array) && !is_jumping)
	{
		is_falling = 1;
		character_position = next_pos;
		jump_actual_high = 0;
	}
	else if(check_character_collision(next_pos, 16, collision_array) && !is_jumping)
	{
		is_falling = 0;
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

bool Character::check_character_collision(const sf::Vector2f position, const int tiles_in_row, std::array<bool,128> collision_array)
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
	return 0;
}

void Character::check_velocity(sf::Time& dt, std::array<bool, 128> collision_array)
{
	on_ground = 0;
	velocity_y += gravity * dt.asSeconds();
	float change = std::min(velocity_y, max_fall_speed);
	sf::Vector2f next_pos = { character_position.x, character_position.y + (change * dt.asSeconds()) };
	bool collided = check_character_collision(next_pos, 16, collision_array);
	if (!collided)
	{

		character_position = next_pos;

	}
	else
	{
		if (velocity_y > 0.f)
		{
			on_ground = 1;
		}
		else
		{
			velocity_y = 0.f;

		}
	}
	is_falling = (!on_ground && velocity_y > 0.f);
	is_jumping = (!on_ground && velocity_y < 0.f);
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

			if (attack_time <= sf::seconds(0.3f))
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

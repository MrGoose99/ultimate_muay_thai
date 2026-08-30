#include "Menu_fighter.hpp"
#include <iostream>
#include <cstdlib>
#include "Punching_bag.hpp"

Menu_fighter::Menu_fighter(sf::Vector2f position, sf::Vector2f scale)
{
	if(!character_texture.loadFromFile("textures/player_textures_new.png"))
		std::cout << "Error loading menu fighter texture\n";

	character_name = "menu_fighter";

	character_sprite.setTexture(character_texture);
	character_sprite.setTextureRect(get_frame_position(2));
	right_side = 1;
	animation_stage = 0;
	moving_normal = 0;
	moving_fight = 0;
	is_falling = 0;
	is_jumping = 0;
	on_ground = 1;
	is_fighting = true;
	character_sprite.setPosition(position);
	character_sprite.setScale(scale);

	attack_time = sf::seconds(0.f);
}

void Menu_fighter::update_character_animation(sf::Time& dt)
{


	sf::Color color = character_sprite.getColor();
	if (transparenting_status == 0)
	{
		color.a = 255.f;
		character_sprite.setColor(color);
	}
	else
	{
		color.a = 127.f;
		character_sprite.setColor(color);
	}


	if (attacked)
		character_sprite.setTextureRect(get_frame_position(26));
	else if (is_jumping)
	{
		if (animation_stage == 0)
			character_sprite.setTextureRect(get_frame_position(13));
		else if (animation_stage == 1)
			character_sprite.setTextureRect(get_frame_position(14));
	}
	else if (is_falling)
	{
		if (animation_stage == 0)
			character_sprite.setTextureRect(get_frame_position(10));
		else if (animation_stage == 1)
			character_sprite.setTextureRect(get_frame_position(11));
	}
	else if (is_fighting)
	{
		if (is_blocking)
			character_sprite.setTextureRect(get_frame_position(4));
		else if (moving_fight > 0 && moving_normal == 0)
		{
			if (animation_stage == 0)
				character_sprite.setTextureRect(get_frame_position(8));
			else if (animation_stage == 1)
				character_sprite.setTextureRect(get_frame_position(9));
		}
		else if (on_ground)
		{
			if (meele_attack_state == MeeleAttackState::None && kick_attack_state == KickAttackState::None)
			{
				if (animation_stage == 0)
					character_sprite.setTextureRect(get_frame_position(2));
				else if (animation_stage == 1)
					character_sprite.setTextureRect(get_frame_position(3));
			}
			else if (meele_attack_state != MeeleAttackState::None)
			{
				switch (animation_stage)
				{
				case 1:character_sprite.setTextureRect(get_frame_position(15)); break;
				case 2:character_sprite.setTextureRect(get_frame_position(16)); break;
				case 3:character_sprite.setTextureRect(get_frame_position(17)); break;
				case 4:character_sprite.setTextureRect(get_frame_position(18)); break;
				case 5:character_sprite.setTextureRect(get_frame_position(19)); break;
				case 6:character_sprite.setTextureRect(get_frame_position(20)); break;
				default: break;
				}
			}
			else if (kick_attack_state != KickAttackState::None)
			{
				switch (animation_stage)
				{
				case 1:character_sprite.setTextureRect(get_frame_position(21)); break;
				case 2:character_sprite.setTextureRect(get_frame_position(22)); break;
				case 3:character_sprite.setTextureRect(get_frame_position(23)); break;
				case 4:character_sprite.setTextureRect(get_frame_position(24)); break;
				case 5:character_sprite.setTextureRect(get_frame_position(25)); break;
				case 6:character_sprite.setTextureRect(get_frame_position(2)); break;
				default: break;
				}
			}
		}
	}
	else
	{
		if (moving_normal > 0 && moving_fight == 0)
		{
			if (animation_stage == 0)

					character_sprite.setTextureRect(get_frame_position(33));
			else if (animation_stage == 1)

					character_sprite.setTextureRect(get_frame_position(34));
			else if (animation_stage == 2)

					character_sprite.setTextureRect(get_frame_position(35));
		}
		else if (on_ground)
		{
			if (animation_stage == 0)

					character_sprite.setTextureRect(get_frame_position(29));
			else if (animation_stage == 1)
;
				else character_sprite.setTextureRect(get_frame_position(30));
		}
	}
}

void Menu_fighter::update_frame_status(sf::Time& dt)
{
	time_animation += dt;
	//std::cout << "animation_stage = " << animation_stage << std::endl;
	//std::cout << "moving_normal = " << moving_normal << std::endl << "moving_fight = " << moving_fight << std::endl;
	//if(on_ground) std::cout << "on_ground = " << on_ground << std::endl;

	if (knocked)
	{
		transparenting_time += dt;
		if (transparenting_time >= sf::seconds(0.15f))
		{
			if (transparenting_status == 0)
				transparenting_status = 1;
			else transparenting_status = 0;
			transparenting_time = sf::seconds(0.f);
		}
	}
	else transparenting_status = 0;


	if (is_jumping) //jumping
	{
		if (time_animation >= sf::seconds(0.3f))
		{
			if (animation_stage == 1)
				animation_stage = 0;
			else animation_stage = 1;
			time_animation = sf::seconds(0.f);
		}
	}
	else if (is_falling) //falling
	{
		if (time_animation >= sf::seconds(0.3f))
		{
			if (animation_stage == 1)
				animation_stage = 0;
			else animation_stage = 1;
			time_animation = sf::seconds(0.f);
		}
	}
	else if (is_fighting && !is_blocking && on_ground && moving_normal == 0 && moving_fight == 0) // default fighting
	{
		if (meele_attack_state == MeeleAttackState::None && kick_attack_state == KickAttackState::None)
		{
			if (animation_stage > 1) animation_stage = 0;
			if (time_animation >= sf::seconds(0.5f))
			{
				if (animation_stage == 1)
					animation_stage = 0;
				else animation_stage = 1;
				time_animation = sf::seconds(0.f);
			}
		}
		else if (meele_attack_state != MeeleAttackState::None)
		{
			if (meele_attack_state == MeeleAttackState::Attack1)
			{
				if (attack_time <= sf::seconds(0.15f))
				{
					animation_stage = 1;
				}
				else animation_stage = 2;
			}
			else if (meele_attack_state == MeeleAttackState::Attack2)
			{
				if (attack_time <= sf::seconds(0.15f))
				{
					animation_stage = 3;
				}
				else animation_stage = 4;
			}
			else if (meele_attack_state == MeeleAttackState::Attack3)
			{
				if (attack_time <= sf::seconds(0.15f))
					animation_stage = 5;
				else animation_stage = 6;
			}
		}
		else if (kick_attack_state != KickAttackState::None)
		{
			switch (kick_attack_state)
			{
			case KickAttackState::AttackHigh:
			{
				if (attack_time <= sf::seconds(0.15f))
					animation_stage = 1;
				else if (attack_time <= sf::seconds(0.25f) && attack_time >= sf::seconds(0.15f))
					animation_stage = 2;
				else if (attack_time >= sf::seconds(0.25f))
					animation_stage = 3;
				break;
			}
			case KickAttackState::AttackMiddle:
			{
				if (attack_time < sf::seconds(0.15f))
					animation_stage = 1;
				else if (attack_time >= sf::seconds(0.15f))
					animation_stage = 4;
			}
			break;
			case KickAttackState::AttackLow:
			{
				if (attack_time < sf::seconds(0.2f))
					animation_stage = 5;
				else
					animation_stage = 6;
				break;
			}
			default:
				break;
			}
		}

	}
	else if (is_fighting && !is_blocking && on_ground && moving_normal == 0 && moving_fight > 0) //moving fighting
	{
		if (animation_stage > 1) animation_stage = 0;
		if (time_animation >= sf::seconds(0.15f))
		{
			if (animation_stage == 1)
				animation_stage = 0;
			else animation_stage = 1;
			time_animation = sf::seconds(0.f);
		}
	}
	else if (!is_fighting && !is_blocking && on_ground && moving_normal == 0 && moving_fight == 0) //default normal
	{
		if (animation_stage > 1) animation_stage = 0;
		if (time_animation >= sf::seconds(0.7f))
		{
			if (animation_stage == 1)
				animation_stage = 0;
			else animation_stage = 1;
			time_animation = sf::seconds(0.f);
		}
	}
	else if (!is_fighting && !is_blocking && on_ground && moving_normal > 0 && moving_fight == 0) //moving normal
	{
		if (time_animation >= sf::seconds(0.15f))
		{
			if (animation_stage == 0)
				animation_stage = 1;
			else if (animation_stage == 1 && !moving_flag)
			{
				animation_stage = 2;
				moving_flag = 1;
			}
			else if (animation_stage == 1 && moving_flag)
			{
				animation_stage = 0;
				moving_flag = 0;
			}
			else if (animation_stage == 2)
				animation_stage = 1;
			time_animation = sf::seconds(0.f);
		}
	}
}

void Menu_fighter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(character_sprite, states);
}
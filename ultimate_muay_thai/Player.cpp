#include "Player.hpp"
#include "Character.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>


Player::Player()
{
	is_fighting = false;
	is_moving = false;
	is_blocking = false;
	animation_stage = 0;
	character_name = "Player";
	if (!character_texture.loadFromFile("textures/player_textures.png"))
		std::cout << "Error loading player texture from file\n";
	character_sprite.setTexture(character_texture);
	character_position = { 100.f, 100.f };
	character_sprite.setPosition(character_position);
	character_sprite.setTextureRect(get_frame_position(0));
	hitbox.size = { 64,128 };
	hitbox.position = { character_position.x + character_sprite.getLocalBounds().size.x / 2 - hitbox.size.x / 2, character_position.y };
	right_side = true;
	moving_normal = 0;
	moving_fight = 0;
	is_falling = 0;
	is_jumping = 0;
	on_ground = 0;


}

void Player::update_character_animation(sf::Time& dt)
{
	if (right_side)
	{
		character_sprite.setScale({ 1.0f, 1.0f });
		character_sprite.setOrigin({ 0.f, 0.f });
	}
	else
	{
		character_sprite.setScale({ -1.0f, 1.0f });
		character_sprite.setOrigin({ 128.f, 0.f });
	}


	if (is_jumping)
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
		else if(on_ground)
		{
			if (animation_stage == 0)
				character_sprite.setTextureRect(get_frame_position(2));
			else if (animation_stage == 1)
				character_sprite.setTextureRect(get_frame_position(3));
		}
		
	}
	else
	{
		if (moving_normal > 0 && moving_fight == 0)
		{
			if (animation_stage == 0)
				character_sprite.setTextureRect(get_frame_position(5));
			else if (animation_stage == 1)
				character_sprite.setTextureRect(get_frame_position(6));
			else if (animation_stage == 2)
				character_sprite.setTextureRect(get_frame_position(7));
		}
		else if(on_ground)
		{
			if (animation_stage == 0)
				character_sprite.setTextureRect(get_frame_position(0));
			else if (animation_stage == 1)
				character_sprite.setTextureRect(get_frame_position(1));
		}
	}
}

void Player::update_frame_status(sf::Time& dt)
{
	time_animation += dt;
	//std::cout << "animation_stage = " << animation_stage << std::endl;
	//std::cout << "moving_normal = " << moving_normal << std::endl << "moving_fight = " << moving_fight << std::endl;
	//if(on_ground) std::cout << "on_ground = " << on_ground << std::endl;


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
	else if (is_fighting && !is_blocking  && on_ground && moving_normal == 0 && moving_fight == 0) // default fighting
	{
		if (animation_stage > 1) animation_stage = 0;
		if(time_animation >= sf::seconds(0.5f))
		{ 
			if (animation_stage == 1)
				animation_stage = 0;
			else animation_stage = 1;
			time_animation = sf::seconds(0.f);
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
	else if(!is_fighting && !is_blocking && on_ground && moving_normal == 0 && moving_fight == 0 ) //default normal
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
	else if (!is_fighting && !is_blocking && on_ground && moving_normal > 0 && moving_fight == 0 ) //moving normal
	{
		if(time_animation >= sf::seconds(0.15f))
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

bool Player::get_is_fighting() const
{
	return is_fighting;
}

void Player::set_is_fighting(const bool status)
{
	is_fighting = status;
}

bool Player::get_is_moving() const
{
	return is_moving;
}

void Player::set_is_moving(const bool status)
{
	is_moving = status;
}

bool Player::get_is_blocking() const
{
	return is_blocking;
}

void Player::set_is_blocking(const bool status)
{
	is_blocking = status;
}

void Player::check_player_events(const std::optional<sf::Event>& event, sf::Time& dt)
{
	if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->scancode == sf::Keyboard::Scancode::Space && !is_jumping && !is_falling)
		{
			if (is_fighting)
				is_fighting = false;
			else is_fighting = true;
		}
		if(is_fighting)
			if (keyPressed->scancode == sf::Keyboard::Scancode::B)
			{
				is_blocking = true;
			}
		if (keyPressed->scancode == sf::Keyboard::Scancode::W && !is_falling && !is_fighting && !is_jumping)
		{
				is_jumping = 1;
				on_ground = 0;
				velocity_y = -1000.f;
		}
	}
}
void Player::check_pressed()
{
	is_blocking = is_fighting && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::B);

	if (!is_fighting)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
		{
			moving_normal = 2;
			moving_fight = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
		{
			moving_normal = 1;
			moving_fight = 0;
		}
		else moving_normal = 0;
	}
	else if(is_fighting && !is_falling && !is_jumping)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
		{
			moving_fight = 2;
			moving_normal = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
		{
			moving_fight = 1;
			moving_normal = 0;
		}
		else moving_fight = 0;
	}

}

sf::Vector2f Player::get_player_position()
{
	return character_position;
}

sf::Vector2f Player::get_player_center()
{
	return sf::Vector2f{ hitbox.position.x + (hitbox.size.x / 2), hitbox.position.y + (hitbox.size.y / 2) };
}



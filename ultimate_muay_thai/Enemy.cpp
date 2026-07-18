#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include "Character.hpp"
#include "Interactive.hpp"
#include "Player.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>

Enemy::Enemy(int starting, std::vector<std::vector<Interactive*>>& moving_objects, int tiles_in_row)
{
	spawning_tile = starting;
	character_name = "enemy";
	Interactive::hp = 10;
	if (!character_texture.loadFromFile("textures/enemy_lvl_1_textures.png"))
		std::cout << "Error loading enemy texture...\n";
	character_sprite.setTexture(character_texture);

	sf::IntRect tex_rect;
	tex_rect.position = { 0,0 }; tex_rect.size = { 128, 128 };
	character_sprite.setTextureRect(tex_rect);

	enemy_state = EnemyState::None;

	float pos_x = starting % tiles_in_row * 128.f; float pos_y = starting / tiles_in_row * 128.f;
	character_position ={ pos_x, pos_y };
	Character::hitbox.size = { 64,128 - 128 / 8 };
	Character::hitbox.position = { character_position.x + character_sprite.getLocalBounds().size.x / 2 - Character::hitbox.size.x / 2, character_position.y + character_sprite.getLocalBounds().size.y / 7 };

	moving_objects[starting].push_back(this);
	actual_tiles.push_back(starting);
}

void Enemy::update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level)
{
	state_update(p1);
	switch (enemy_state)
	{
	case EnemyState::None:
	{
		moving_normal = 0;
		is_fighting = 0;
		moving_fight = 0;
		break;
	}
	case EnemyState::Patroling:
	{
		is_fighting = 0;
		moving_fight = 0;
		float min_x = patrol_tile % tiles_in_row * 128.f;
		float max_x = patrol_tile % tiles_in_row * 128.f + 127.f;
		if (this->get_character_position().x <= min_x) patroling_direction = 1;
		else if (this->get_character_position().x >= max_x) patroling_direction = 0;
		if (patroling_direction) moving_normal = 2;
		else moving_normal = 1;
		std::cout << "moving_normal = " << moving_normal << std::endl;
		break;
	}
	case EnemyState::Rushing:
	{
		is_fighting = 0;
		moving_fight = 0;
		if (p1.get_character_position().x < this->get_character_position().x) moving_normal = 1;
		else if (p1.get_character_position().x > this->get_character_position().x) moving_normal = 2;
		break;
	}
	case EnemyState::Attacking:
	{
		is_fighting = 1;
		is_blocking = 0;
		moving_normal = 0;
		moving_fight = 0;
		if (p1.get_character_position().x < this->get_character_position().x) right_side = 0;
		else right_side = 1;
		if (attack_time == sf::seconds(0.f))
		{
			if (meele_attack_state == MeeleAttackState::None && kick_attack_state == KickAttackState::None)
			{
				if (std::rand() % 100 < 50)
				{
					meele_attack_state = MeeleAttackState::Attack1;
					kick_attack_state = KickAttackState::None;
				}
				else
				{
					int random = std::rand() % 100;
					if (random <= 100.f / 3)
						kick_attack_state = KickAttackState::AttackHigh;
					else if (random > 100.f / 3 && random < 100.f / 3 * 2)
						kick_attack_state = KickAttackState::AttackMiddle;
					else kick_attack_state = KickAttackState::AttackLow;
					meele_attack_state = MeeleAttackState::None;
				}
			}
			else if (meele_attack_state == MeeleAttackState::Attack1 && kick_attack_state == KickAttackState::None)
			{
				if (std::rand() % 100 < 50)
					meele_attack_state = MeeleAttackState::Attack2;
				else meele_attack_state = MeeleAttackState::None;
			}
			else if (meele_attack_state == MeeleAttackState::Attack2 && kick_attack_state == KickAttackState::None)
			{
				if (std::rand() % 100 < 50)
					meele_attack_state = MeeleAttackState::Attack3;
				else meele_attack_state = MeeleAttackState::None;
			}
			else if (meele_attack_state == MeeleAttackState::Attack3 && kick_attack_state == KickAttackState::None)
				meele_attack_state = MeeleAttackState::None;
			else if (kick_attack_state != KickAttackState::None && meele_attack_state == MeeleAttackState::None)
				kick_attack_state = KickAttackState::None;
		}
		else
		{
			if (attack_time < sf::seconds(0.3f))
				attack_time += dt;
			else attack_time = sf::seconds(0.f);
		}
		break;
	}
	case EnemyState::Blocking:
	{
		//BLOCKING CODE
		break;
	}
	default:
		moving_normal = 0;
		is_fighting = 0;
		moving_fight = 0;
	}
	//std::cout << "Collision_array_size: " << collision_array.size() << std::endl;
	character_moving(dt, moving_speed, 75.f, collision_array, moving_objects, tiles_in_row, tiles_in_level);
	check_velocity_y(dt, collision_array, tiles_in_row, moving_objects, tiles_in_level);
	character_position_update();
	interactive_grid_update(moving_objects, tiles_in_row);
	attack(dt);
	update_frame_status(dt);
	update_character_animation(dt);

	//DEBUG
	//std::cout << "right_side: " << right_side << std::endl; 
	std::cout << "attackbox_active: " << attackbox_active << std::endl;
	//std::cout << "attack_time: " << attack_time.asSeconds() << std::endl;
	//std::cout << "actual_tiles: ";
	//	for(int i = 0; i < actual_tiles.size(); i++)
	//	std::cout << actual_tiles[i] << " ";
	//	std::cout << std::endl;
	//std::cout << "actual_status: " << (int)enemy_state << std::endl;
	//////////
}

void Enemy::state_update(Player& p1)
{
	switch (enemy_state)
	{
	case EnemyState::None:
		{
		if (std::abs(p1.get_character_position().x - this->get_character_position().x) <= 2000.f || std::abs(p1.get_character_position().y - this->get_character_position().y) <= 2000.f)
		{
			enemy_state = EnemyState::Patroling;
			patrol_tile = actual_tiles[0];
		}
			break;
		}
	case EnemyState::Patroling:
	{
		if (std::abs(p1.get_character_position().x - this->get_character_position().x) > 2000.f || std::abs(p1.get_character_position().y - this->get_character_position().y) > 2000.f)
			enemy_state = EnemyState::None;
		else if (std::abs(p1.get_character_position().x - this->get_character_position().x) <= 600.f && std::abs(p1.get_character_position().y - this->get_character_position().y) < 256.f)
			enemy_state = EnemyState::Rushing;
		break;
	}
	case EnemyState::Rushing:
	{
		if (std::abs(p1.get_character_position().x - this->get_character_position().x) > 600.f || std::abs(p1.get_character_position().y - this->get_character_position().y) >= 256.f)
		{
			enemy_state = EnemyState::Patroling;
			patrol_tile = actual_tiles[0];
		}
		else if (std::abs(p1.get_character_position().x - this->get_character_position().x) <= 90.f && std::abs(p1.get_character_position().y - this->get_character_position().y) < 100.f)
			enemy_state = EnemyState::Attacking;
		break;
	}
	case EnemyState::Attacking:
	{
		if (std::abs(p1.get_character_position().x - this->get_character_position().x) > 90.f || std::abs(p1.get_character_position().y - this->get_character_position().y) >= 100.f)
			enemy_state = EnemyState::Rushing;
		break;
	}
	case EnemyState::Blocking:
	{
		// BLOCKING CODE
		break;
	}
	default:
		enemy_state = EnemyState::Patroling;
	}
}

void Enemy::interactive_grid_update(std::vector<std::vector<Interactive*>>& interactive_grid, int tiles_in_row)
{
	for (int i = 0; i < actual_tiles.size(); i++)
	{
	int tile = actual_tiles[i];
	interactive_grid[tile].erase(std::remove(interactive_grid[tile].begin(), interactive_grid[tile].end(), this), interactive_grid[tile].end());
	}
	actual_tiles.clear();

	int left = character_position.x / 128.f;
	int right = (character_position.x + character_sprite.getLocalBounds().size.x) / 128.f;
	int top = character_position.y / 128.f;
	int bottom = (character_position.y + character_sprite.getLocalBounds().size.y) / 128.f;

	for (int y = top; y <= bottom; y++)
		for (int x = left; x <= right; x++)
		{

			if (x < 0 || y < 0 || x >= tiles_in_row || y >= interactive_grid.size() / tiles_in_row)
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
					interactive_grid[checking_tile].push_back(this);
				}
			}
		}
}

void Enemy::texture_update(sf::Time& dt)
{

}

void Enemy::update_character_animation(sf::Time& dt)
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
				character_sprite.setTextureRect(get_frame_position(5));
			else if (animation_stage == 1)
				character_sprite.setTextureRect(get_frame_position(6));
			else if (animation_stage == 2)
				character_sprite.setTextureRect(get_frame_position(7));
		}
		else if (on_ground)
		{
			if (animation_stage == 0)
				character_sprite.setTextureRect(get_frame_position(0));
			else if (animation_stage == 1)
				character_sprite.setTextureRect(get_frame_position(1));
		}
	}

}

void Enemy::update_frame_status(sf::Time& dt)
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

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(character_sprite, states);
}

std::string Enemy::get_object_type()
{
	return type;
}

const sf::Sprite& Enemy::get_object_sprite() const
{
	return character_sprite;
}
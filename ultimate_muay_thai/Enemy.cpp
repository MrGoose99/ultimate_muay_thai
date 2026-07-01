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
				}
				else
				{
					int random = std::rand() % 100;
					if (random <= 100.f / 3)
						kick_attack_state = KickAttackState::AttackHigh;
					else if (random > 100.f / 3 && random < 100.f / 3 * 2)
						kick_attack_state = KickAttackState::AttackMiddle;
					else kick_attack_state = KickAttackState::AttackLow;
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
	character_moving(dt, moving_speed, 75.f, collision_array, moving_objects, tiles_in_row, tiles_in_level);
	//check_velocity_y(dt, collision_array, tiles_in_row, moving_objects, tiles_in_level);
	character_position_update();
	interactive_grid_update(moving_objects, tiles_in_row);

	//DEBUG
	std::cout << "actual_tiles: ";
	for(int i = 0; i < actual_tiles.size(); i++)
		std::cout << actual_tiles[i] << " ";
	std::cout << std::endl;

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
		else if (std::abs(p1.get_character_position().x - this->get_character_position().x) <= 300.f && std::abs(p1.get_character_position().y - this->get_character_position().y) < 128.f)
			enemy_state = EnemyState::Rushing;
		break;
	}
	case EnemyState::Rushing:
	{
		if (std::abs(p1.get_character_position().x - this->get_character_position().x) > 300.f || std::abs(p1.get_character_position().y - this->get_character_position().y) >= 128.f)
		{
			enemy_state = EnemyState::Patroling;
			patrol_tile = actual_tiles[0];
		}
		else if (std::abs(p1.get_character_position().x - this->get_character_position().x) <= 15.f && std::abs(p1.get_character_position().y - this->get_character_position().y) < 64.f)
			enemy_state = EnemyState::Attacking;
		break;
	}
	case EnemyState::Attacking:
	{
		if (std::abs(p1.get_character_position().x - this->get_character_position().x) > 15.f || std::abs(p1.get_character_position().y - this->get_character_position().y) >= 64.f)
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

}
void Enemy::update_frame_status(sf::Time& dt)
{

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
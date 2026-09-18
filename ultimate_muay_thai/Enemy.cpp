#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include "Character.hpp"
#include "Interactive.hpp"
#include "Player.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>

Enemy::Enemy(int starting, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::vector<Character*>>& character_grid, int tiles_in_row)
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
	character_position = { pos_x, pos_y };
	Character::hitbox.size = { 64,128 - 128 / 8 };
	Character::hitbox.position = { character_position.x + character_sprite.getLocalBounds().size.x / 2 - Character::hitbox.size.x / 2, character_position.y + character_sprite.getLocalBounds().size.y / 7 };

	moving_objects[starting].push_back(this);
	character_grid[starting].push_back(this);
	actual_tiles.push_back(starting);

	punched = 0;

	attackbox.position = { pos_x, pos_y };
	attackbox.size = { 1,1 };
	attackbox_active = 0;
}

void Enemy::update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid)
{
	state_update(p1);
	switch (enemy_state)
	{
	case EnemyState::None:
	{
		patroling_time = sf::seconds(0);
		moving_normal = 0;
		attack_time = sf::seconds(0);
		is_fighting = 0;
		moving_fight = 0;
		break;
	}
	case EnemyState::Patroling:
	{
		is_fighting = 0;
		moving_fight = 0;
		attack_time = sf::seconds(0.f);
		float min_x = patrol_tile % tiles_in_row * 128.f;
		float max_x = patrol_tile % tiles_in_row * 128.f + 127.f;
		if (this->get_character_position().x <= min_x)
		{
			patroling_time += dt;
			if (patroling_time < sf::seconds(3))
				patroling_direction = 0;
			else
			{
				patroling_direction = 2;
				patroling_time = sf::seconds(0);
				this->set_character_position(min_x + 1, this->get_character_position().y);
			}
		}
		else if (this->get_character_position().x >= max_x)
		{
			patroling_time += dt;
			if (patroling_time < sf::seconds(3))
				patroling_direction = 0;
			else
			{
				patroling_direction = 1;
				patroling_time = sf::seconds(0);
				this->set_character_position(max_x - 1, this->get_character_position().y);
			}
		}
		if (patroling_direction == 2) moving_normal = 2;
		else if (patroling_direction == 1) moving_normal = 1;
		else moving_normal = 0;
		break;
	}
	case EnemyState::Rushing:
	{
		patroling_time = sf::seconds(0);
		attack_time = sf::seconds(0);
		is_fighting = 0;
		moving_fight = 0;

		punched = 1;
		attacked_time = sf::seconds(0.f);

		if (p1.get_character_position().x < this->get_character_position().x) moving_normal = 1;
		else if (p1.get_character_position().x > this->get_character_position().x) moving_normal = 2;

		break;
	}
	case EnemyState::CloseApproach:
	{
		is_fighting = 1;
		Interactive::is_blocking = 0;
		moving_normal = 0;

		if (punched != 0) attacked_time += dt;
		else attacked_time = sf::seconds(0.f);

		if (punched != 0)
		{
			if (attacked_time >= sf::seconds(0.4f))
			{
				punched = 0;
			}
		}

		if (p1.get_character_position().x < this->get_character_position().x) moving_fight = 1;
		else if (p1.get_character_position().x > this->get_character_position().x) moving_fight = 2;

		meele_attack_state = MeeleAttackState::None; kick_attack_state = KickAttackState::None;
		attackbox_active = 0;

		break;
	}
	case EnemyState::Attacking:
	{
		is_fighting = 1;
		Interactive::is_blocking = false;
		moving_normal = 0;
		moving_fight = 0;
		patroling_time = sf::seconds(0.f);
		blocking_time = sf::seconds(0.f);

		if(time_to_attack < sf::seconds(0.5f)) time_to_attack += dt;
		if (punched != 0) attacked_time += dt;
		else attacked_time = sf::seconds(0.f);

		if (p1.get_character_position().x < this->get_character_position().x) right_side = 0;
		else right_side = 1;

		if (punched != 0)
		{
			if (attacked_time >= sf::seconds(0.7f))
			{
				punched = 0;
				time_to_attack = sf::seconds(0.f);
				attack_time = sf::seconds(0.f);
			}
			else 
				attackbox_active = 0;
		}
		else if (attack_time == sf::seconds(0.f) && time_to_attack >= sf::seconds(0.5f))
		{
			if (meele_attack_state == MeeleAttackState::None && kick_attack_state == KickAttackState::None)
			{
				if (std::rand() % 100 < 60)
				{
					int random = std::rand() % 100;
					if (random <= 100 / 3)
						meele_attack_state = MeeleAttackState::Attack1;
					else if (random > 100 / 3 && random < 100 / 3 * 2)
						meele_attack_state = MeeleAttackState::Attack2;
					else meele_attack_state = MeeleAttackState::Attack3;
					kick_attack_state = KickAttackState::None;

				}
				else
				{
					int random = std::rand() % 100;
					if (random <= 100 / 3)
						kick_attack_state = KickAttackState::AttackHigh;
					else if (random > 100 / 3 && random < 100 / 3 * 2)
						kick_attack_state = KickAttackState::AttackMiddle;
					else kick_attack_state = KickAttackState::AttackLow;
					meele_attack_state = MeeleAttackState::None;

				}
			}
			else if (kick_attack_state != KickAttackState::None && meele_attack_state == MeeleAttackState::None)
			{
				kick_attack_state = KickAttackState::None;
				time_to_attack = sf::seconds(0.f);
			}
		}
		else
		{
			if (attack_time >= sf::seconds(3.f))
			{
				attack_time = sf::seconds(0.f);

				time_to_attack = sf::seconds(0.f);
			}
		
		}
		//std::cout << "HP = " << Interactive::hp << std::endl;
		//std::cout << "PUNCHED = " << punched << std::endl;
		break;
	}
	case EnemyState::Blocking:
	{
		attackbox_active = 0;
		Interactive::is_blocking = true;
		blocking_time += dt;
		break;
	}
	case EnemyState::Dying:
	{
		moving_normal = 0;
		moving_fight = 0;
		dying_time += dt;
		if (dying_time >= sf::seconds(10.f)) is_destroyed = true;
		attackbox_active = 0;
		break;
	}
	default:
		moving_normal = 0;
		is_fighting = 0;
		moving_fight = 0;
		meele_attack_state = MeeleAttackState::None;
		kick_attack_state = KickAttackState::None;
		attack_time = sf::seconds(0.f);
		patroling_time = sf::seconds(0.f);
	}
	//std::cout << "Collision_array_size: " << collision_array.size() << std::endl;
	character_moving(dt, moving_speed, 75.f, collision_array, moving_objects, tiles_in_row, tiles_in_level, character_grid);
	check_velocity_y(dt, collision_array, tiles_in_row, moving_objects, tiles_in_level, character_grid);
	character_position_update();
	grid_update(moving_objects, character_grid, tiles_in_row);
	if(enemy_state == EnemyState::Attacking && time_to_attack >= sf::seconds(0.3f)) attack(dt);
	update_frame_status(dt);
	update_character_animation(dt);

	//DEBUG
	//std::cout << "right_side: " << right_side << std::endl; 
	//std::cout << "attackbox_active: " << attackbox_active << std::endl;
	//std::cout << "actual_tiles: ";
	//	for(int i = 0; i < actual_tiles.size(); i++)
	//	std::cout << actual_tiles[i] << " ";
	//	std::cout << std::endl;
	/*std::cout << "actual_status: " << (int)enemy_state << std::endl;
	std::cout << "attack_time = " << attack_time.asSeconds() << std::endl;
	std::cout << "MeeleAttackState::";
	switch (meele_attack_state)
	{
	case MeeleAttackState::None:
		std::cout << "None\n";
		break;
	case MeeleAttackState::Attack1:
		std::cout << "Attack1\n";
		break;
	case MeeleAttackState::Attack2:
		std::cout << "Attack2\n";
		break;
	case MeeleAttackState::Attack3:
		std::cout << "Attack3\n";
		break;
	default:
		std::cout << "None";
	}
	std::cout << "KickAttackState::";
	switch (kick_attack_state)
	{
	case KickAttackState::AttackHigh:
		std::cout << "AttackHigh\n";
		break;
	case KickAttackState::AttackMiddle:
		std::cout << "AttackMiddle\n";
		break;
	case KickAttackState::AttackLow:
		std::cout << "AttackLow\n";
		break;
	case KickAttackState::None:
		std::cout << "None\n";
		break;
	default:
		std::cout << "None\n";
	}	*/
	//////////
}

void Enemy::state_update(Player& p1)
{
	if (is_dying)
	{
		enemy_state = EnemyState::Dying;
		character_is_dying = true;
	}
	else
	{
		switch (enemy_state)
		{
		case EnemyState::None:
		{
			if (std::abs(p1.get_character_position().x - this->get_character_position().x) <= 2000.f || std::abs(p1.get_character_position().y - this->get_character_position().y) <= 2000.f)
			{
				enemy_state = EnemyState::Patroling;
				patroling_direction = 1;
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
				patroling_direction = 1;
			}
			else if (std::abs(p1.get_character_position().x - this->get_character_position().x) <= 120.f && std::abs(p1.get_character_position().y - this->get_character_position().y) < 100.f)
			{
				enemy_state = EnemyState::CloseApproach;
				punched = 0;
			}
			break;
		}
		case EnemyState::CloseApproach:
		{
			if (std::abs(p1.get_character_position().x - this->get_character_position().x) > 120.f || std::abs(p1.get_character_position().y - this->get_character_position().y) >= 100.f)
				enemy_state = EnemyState::Rushing;
			else if (std::abs(p1.get_character_position().x - this->get_character_position().x) <= 90.f && std::abs(p1.get_character_position().y - this->get_character_position().y) < 100.f)
			{
				enemy_state = EnemyState::Attacking;
				time_to_attack = sf::seconds(0.f);
			}
			break;
		}
		case EnemyState::Attacking:
		{
			if (std::abs(p1.get_character_position().x - this->get_character_position().x) > 90.f)
			{
				enemy_state = EnemyState::CloseApproach;
				animation_stage = 0; time_animation = sf::seconds(0.f); //reseting animation to not freeze after change to rushing durning attack
			}
			else if (p1.get_starting_strike())
			{
				int random = rand() % 100;
				p1.set_starting_strike(false);
				if (random <= 40)
				{
					enemy_state = EnemyState::Blocking;
				}
			}
			break;
		}
		case EnemyState::Blocking:
		{
			if (blocking_time >= sf::seconds(0.7f))
			{
				blocking_time = sf::seconds(0.f);
				enemy_state = EnemyState::Attacking;

			}
			if (p1.get_starting_strike())
			{
					p1.set_starting_strike(false);
					blocking_time = sf::seconds(0.f);
			}
			else if (!p1.get_starting_strike() && rand() % 100 == 0)
			{
				attack_time = sf::seconds(2.f);
				p1.set_attack_latency_to_zero();
				p1.set_meele_attack_state_to_none();
				enemy_state = EnemyState::Attacking;
			}
			break;
		}
		default:
			enemy_state = EnemyState::Patroling;
		}
	}
}

void Enemy::grid_update(std::vector<std::vector<Interactive*>>& interactive_grid, std::vector<std::vector<Character*>>& character_grid, int tiles_in_row)
{
	for (int i = 0; i < actual_tiles.size(); i++)
	{
		int tile = actual_tiles[i];
		interactive_grid[tile].erase(std::remove(interactive_grid[tile].begin(), interactive_grid[tile].end(), this), interactive_grid[tile].end());
		character_grid[tile].erase(std::remove(character_grid[tile].begin(), character_grid[tile].end(), this), character_grid[tile].end());
	}
	actual_tiles.clear();
	if (!is_destroyed)
	{
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
						character_grid[checking_tile].push_back(this);
					}
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
	if(is_dying)
	{
		if (dying_time < sf::seconds(0.25f))
			if (!by_bullet)
				character_sprite.setTextureRect(get_frame_position(26));
			else
			{
				character_sprite.setTextureRect(get_frame_position(36));
				if (enemy_hit_dead_sound.getStatus() != sf::SoundSource::Status::Playing)
				{
						enemy_hit_dead_sound.play(); //WARNING - SOUND IN THIS METHOD!!!!
				}
			}
		else if (dying_time < sf::seconds(0.6f))
			character_sprite.setTextureRect(get_frame_position(27));
		else character_sprite.setTextureRect(get_frame_position(28));

		if (dying_time >= sf::seconds(4.f))
		{
			if (dying_transparenting > 0.f)
			{
				dying_transparenting -= 255.f * dt.asSeconds();
			}
			sf::Color color = character_sprite.getColor();
			color.a = dying_transparenting;
			character_sprite.setColor(color);
		}
	}
	else if (punched != 0 && (enemy_state == EnemyState::CloseApproach || enemy_state == EnemyState::Attacking || enemy_state == EnemyState::Blocking))
	{
		character_sprite.setTextureRect(get_frame_position(26));
	}
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
		if (Interactive::is_blocking)
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
				case 6:character_sprite.setTextureRect(get_frame_position(2)); break; //it's not a bug
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
	else if (is_fighting && !Interactive::is_blocking && on_ground && moving_normal == 0 && moving_fight == 0) // default fighting
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
	else if (is_fighting && !Interactive::is_blocking && on_ground && moving_normal == 0 && moving_fight > 0) //moving fighting
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
	else if (!is_fighting && !Interactive::is_blocking && on_ground && moving_normal == 0 && moving_fight == 0) //default normal
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
	else if (!is_fighting && !Interactive::is_blocking && on_ground && moving_normal > 0 && moving_fight == 0) //moving normal
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
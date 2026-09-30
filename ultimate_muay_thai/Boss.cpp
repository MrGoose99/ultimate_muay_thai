#include "Boss.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

Boss::Boss(int starting, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::vector<Character*>>& character_grid, int tiles_in_row)
{
	if (!character_texture.loadFromFile("textures/boss_textures.png"))
		std::cout << "Error loading boss texture...\n";
	character_sprite.setTexture(character_texture);

	sf::IntRect tex_rect;
	tex_rect.position = { 0,0 }; tex_rect.size = { 128, 128 };
	character_sprite.setTextureRect(tex_rect);

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

	boss_hitbox_active = false;
	Interactive::hp = 50;
}

void Boss::update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid)
{
	state_update(p1);
	
	switch (boss_state)
	{
	case BossState::None:
		moving_normal = 0;
		break;

	case BossState::Walking:
		if (p1.get_character_position().x < this->get_character_position().x) moving_normal = 1;
		else if (p1.get_character_position().x > this->get_character_position().x) moving_normal = 2;
		break;

	case BossState::Rush:
		rushing_time += dt;

		if (rushing_time <= sf::seconds(5.f))
		{
			moving_normal = 0;

			if (p1.get_character_position().x < this->get_character_position().x) right_side = 1;
			else right_side = 0;

		}
		else if (rushing_time <= sf::seconds(15.f) && rushing_distance < 640.f)
		{
			if (rushing_distance == 0.f)
				starting_rushing_position = this->get_character_position();

			if(!rushing_attack_done) boss_hitbox_active = true;

			if (!rushing_attack_done && !boss_hitbox_active) rushing_attack_done = true;

			if (!right_side) moving_normal = 1;
			else moving_normal = 2;
			moving_speed = 3750.f;
			rushing_distance += moving_speed * dt.asSeconds();
		}
		break;

	case BossState::Blast:
		blasting_time += dt;
		if ((blasting_time <= sf::seconds(2.f) && blasting_stage == 0) ||
			(blasting_time <= sf::seconds(4.f) && blasting_stage == 1) ||
			(blasting_time <= sf::seconds(6.f) && blasting_stage == 2))
		{
			blasting = true;
			blasting_stage++;
		}
		break;

	case BossState::Unconscious:
		moving_normal = 0;
		immortality = false;
		break;

	case BossState::Dying:
		moving_normal = 0;
		dying_time += dt;
		if (dying_time >= sf::seconds(10.f)) is_destroyed = true;
		break;
	default:
		moving_normal = 0;
		immortality = true;
	}

	character_moving(dt, moving_speed, 75.f, collision_array, moving_objects, tiles_in_row, tiles_in_level, character_grid);
	check_velocity_y(dt, collision_array, tiles_in_row, moving_objects, tiles_in_level, character_grid);
	character_position_update();
	grid_update(moving_objects, character_grid, tiles_in_row);
	update_frame_status(dt);
	update_character_animation(dt);
}

void Boss::texture_update(sf::Time& dt)
{
	//empty method
}

void Boss::update_character_animation(sf::Time& dt)
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

	if (is_dying)
	{
		if (dying_time < sf::seconds(0.25f))
			if (!by_bullet)
				character_sprite.setTextureRect(get_frame_position(26));
			else
			{
				character_sprite.setTextureRect(get_frame_position(36));
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
	else if (punched != 0 && boss_state == BossState::Unconscious)
		character_sprite.setTextureRect(get_frame_position(26));
	else if (moving_normal != 0 && boss_state == BossState::Walking)
	{
		if (animation_stage == 0)
			character_sprite.setTextureRect(get_frame_position(5));
		else if (animation_stage == 1)
			character_sprite.setTextureRect(get_frame_position(6));
		else if (animation_stage == 2)
			character_sprite.setTextureRect(get_frame_position(7));
	}
	else if (boss_state == BossState::Attack)
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
	else if (boss_state == BossState::Rush)
	{
		if (animation_stage == 0)
			character_sprite.setTextureRect(get_frame_position(29));
		else if (animation_stage == 2)
			character_sprite.setTextureRect(get_frame_position(30));
		else if (animation_stage == 3)
			character_sprite.setTextureRect(get_frame_position(31));
	}
	else if (boss_state == BossState::Blast)
	{
		if(animation_stage == 0)
			character_sprite.setTextureRect(get_frame_position(35));
		else if(animation_stage == 1)
			character_sprite.setTextureRect(get_frame_position(34));
	}
	else if (boss_state == BossState::Unconscious)
	{
		if (animation_stage == 0)
			character_sprite.setTextureRect(get_frame_position(32));
		else if (animation_stage == 1)
			character_sprite.setTextureRect(get_frame_position(33));
	}

}

void Boss::update_frame_status(sf::Time& dt)
{
	time_animation += dt;

	if (boss_state == BossState::Attack)
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
	else if (moving_normal == 0 && punched == 0) //default normal
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
	else if (moving_normal != 0 && boss_state == BossState::Walking) //moving normal
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
	else if (boss_state == BossState::Blast && blasting)
	{
		if (time_animation <= sf::seconds(0.5f))
			animation_stage = 0;
		else animation_stage = 1;
	}
	else if (boss_state == BossState::Rush)
	{
		if (time_animation <= sf::seconds(2.f))
			animation_stage = 0;
		else if (time_animation <= sf::seconds(5.f))
			animation_stage = 1;
		else animation_stage = 2;
	}
	else if (boss_state == BossState::Unconscious)
	{
		if (time_animation <= sf::seconds(0.2f))
			animation_stage = 0;
		else animation_stage = 1;
		if (time_animation >= sf::seconds(0.4f))
			time_animation = sf::seconds(0.f);
	}
}

void Boss::state_update(Player& p1)
{
	if (is_dying)
	{
		boss_state = BossState::Dying;
		character_is_dying = true;
	}
	else
	{
		switch (boss_state)
		{
		case BossState::None:
			if (std::abs(p1.get_player_position().x - character_position.x) < 2200.f || std::abs(p1.get_player_position().y - character_position.y) < 1100.f)
				boss_state = BossState::Walking;
			break;

		case BossState::Walking:
			if (std::abs(p1.get_player_position().x - character_position.x) >= 2200.f || std::abs(p1.get_player_position().y - character_position.y) >= 1100.f)
				boss_state = BossState::None;
			else if (std::abs(p1.get_player_position().x - character_position.x) < 150.f)
			{
				BossState::Attack;
				boss_attack_time = sf::seconds(0.f);
			}
			else if (random_attack == 1)
			{
				boss_state = BossState::Rush;
				rushing_time = sf::seconds(0.f);
				rushing_distance = 0.f;
			}
			else if (random_attack == 2)
			{
				boss_state = BossState::Blast;
				blasting_time = sf::seconds(0.f);
			}
			break;

		case BossState::Rush:
			if (rushing_time >= sf::seconds(15.f))
				if (attacks == std::rand() % 3 + 2)
				{
					boss_state = BossState::Unconscious;
					unconscious_time = sf::seconds(0.f);
				}
				else
				{
					boss_state = BossState::Walking;
					attacks++;
				}
			break;
			
		case BossState::Blast:
			if (blasting_time >= sf::seconds(7.f))
				if (attacks == std::rand() % 3 + 2)
				{
					boss_state = BossState::Unconscious;
					unconscious_time = sf::seconds(0.f);
				}
				else
				{
					boss_state = BossState::Walking;
					attacks++;
				}
			break;

		case BossState::Unconscious:
			if (unconscious_time >= sf::seconds(8.f))
			{
				boss_state = BossState::Walking;
				immortality = true;
			}
			break;

		case BossState::Attack:
			if (boss_attack_time >= sf::seconds(1.f))
				boss_state = BossState::Walking;
			break;
		default:
			boss_state = BossState::Walking;
		}
	}

}

void Boss::grid_update(std::vector<std::vector<Interactive*>>& interacitve_grid, std::vector<std::vector<Character*>>& character_grid, int tiles_in_row)
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

std::string Boss::get_object_type()
{
	return type;
}

const sf::Sprite& Boss::get_object_sprite() const
{
	return character_sprite;
}



void Boss::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(character_sprite, states);
}


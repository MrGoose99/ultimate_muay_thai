#include "Character.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Interactive.hpp"
#include "Punching_bag.hpp"


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

void Character::character_moving(sf::Time& dt, const float speed_normal, const float speed_fight, std::vector<bool>& collision_array, std::vector<std::vector<Interactive*>>& interactive_grid, short int tiles_in_row, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid)
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
			if (!check_character_collision(next_pos, tiles_in_row, collision_array, interactive_grid, tiles_in_level, character_grid))
				character_position = next_pos;
		}
		else if (moving_normal == 1)
		{
			if (right_side == 1)
			{
				right_side = 0;
			}
			sf::Vector2f next_pos = { character_position.x + -speed_normal * dt.asSeconds(), character_position.y };
			if(!check_character_collision(next_pos, tiles_in_row, collision_array, interactive_grid, tiles_in_level, character_grid))
				character_position = next_pos;
		}
	}
	else if (is_fighting && !is_falling && !is_jumping)
	{
		if (moving_fight == 2)
		{
			sf::Vector2f next_pos = {character_position.x + speed_fight * dt.asSeconds(), character_position.y };
			if (!check_character_collision(next_pos, tiles_in_row, collision_array, interactive_grid, tiles_in_level, character_grid))
			character_position = next_pos;
		}
		else if (moving_fight == 1)
		{
			sf::Vector2f next_pos = { character_position.x + -speed_fight * dt.asSeconds(), character_position.y };
			if (!check_character_collision(next_pos, tiles_in_row, collision_array, interactive_grid, tiles_in_level, character_grid))
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

void Character::check_collision_with_punching_bag(Punching_bag& punching_bag)
{
	if (attackbox_active && attackbox.findIntersection(punching_bag.get_object_sprite().getGlobalBounds()))
	{
		punching_bag.set_punched(1);
	}
	else punching_bag.set_punched(0);
}

bool Character::check_character_collision(const sf::Vector2f position, const int tiles_in_row, std::vector<bool>& collision_array, std::vector<std::vector<Interactive*>>& interactive_grid, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid)
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
			if (x < 0 || y < 0 || x >= tiles_in_row || y >= tiles_in_level / tiles_in_row)
			{
				continue;
			}
			else if (collision_array[x + y * tiles_in_row])
			{
				sf::FloatRect checking_tile = { {x * 128.f, y * 128.f}, {128,128} };
				if (checking_tile.findIntersection(checking_rect))
				{
					return 1;
				}
			}
		}
		for (int y = top; y <= bottom + 1; y++) //+1 for moving_tile, because it can be on the top of lower tile
			for (int x = left; x <= right; x++)
			{
				if (x < 0 || y < 0 || x >= tiles_in_row || y >= tiles_in_level / tiles_in_row)
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
							sf::FloatRect checking_tile = { interactive_grid[index][i]->get_object_sprite().getGlobalBounds() };
							if (checking_tile.findIntersection(checking_rect))
								return 1;
						}
					}
					for(short int i = 0; i < character_grid[index].size(); i++)
					{
						if (character_grid[index][i] != this && !character_grid[index][i]->get_character_is_dying()) //landing on enemy
						{
							sf::FloatRect tile = { character_grid[index][i]->get_character_hitbox()};
							sf::FloatRect checking_tile = { {tile.position.x + tile.size.x / 4, tile.position.y}, {tile.size.x - tile.size.x / 4 * 2, tile.size.y} };

							if (checking_tile.findIntersection(checking_rect))
							{
								if (checking_rect.position.y + checking_rect.size.y < checking_tile.position.y + checking_tile.size.y / 4)
								{
									if (checking_rect.position.x <= checking_tile.position.x)
									{
										on_ground = 0;
										velocity_x = -600.f;
										velocity_y = -300.f;
										is_jumping = 1;
										return 0;
									}
									else if (checking_rect.position.x + checking_rect.size.x > checking_tile.position.x + checking_tile.size.x)
									{
										on_ground = 0;
										velocity_x = 600.f;
										velocity_y = -300.f;
										is_jumping = 1;
										return 0;
									}
								}
								return 1;
							}
						}
					}
				}
			}
	return 0;
}

void Character::check_velocity_y(sf::Time& dt, std::vector<bool>& collision_array, const int tiles_in_row, std::vector<std::vector<Interactive*>>& interactive_grid, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid)
{
	velocity_y += gravity * dt.asSeconds();
	float change = std::min(velocity_y, max_fall_speed);
	sf::Vector2f next_pos = { character_position.x, character_position.y + (change * dt.asSeconds()) };
	bool collided = check_character_collision(next_pos, tiles_in_row, collision_array, interactive_grid, tiles_in_level, character_grid);
	if (!collided)
	{

		character_position = next_pos;
		if (velocity_y > 0.f)
		{
			falling_time += dt;
			if(falling_time >= sf::seconds(0.1f))  //to prevent issues in animation changes
				is_falling = true;
		}

	}
	else
	{
		falling_time = sf::seconds(0.f);
		on_ground = 1;
		velocity_y = 0.f;
		is_falling = false;

	}
	//is_falling = (!on_ground && velocity_y > 0.f);
	is_jumping = (!on_ground && velocity_y < 0.f);
	std::cout << "on_ground = " << on_ground << std::endl; //DEBUG
	std::cout << "is_falling = " << is_falling << std::endl;
	std::cout << "velocity_y = " << velocity_y << std::endl; //DEBUG
}

void Character::check_velocity_x(sf::Time& dt, std::vector<bool>& collision_array, std::vector<std::vector<Interactive*>>& interactive_grid, const int& tiles_in_level, const int tiles_in_row, std::vector<std::vector<Character*>>& character_grid)
{
	if (velocity_x < 0.f) velocity_x += gravity * dt.asSeconds();
	else if (velocity_x > 0.f) velocity_x -= gravity * dt.asSeconds();
	float change = velocity_x;
	sf::Vector2f next_pos = { character_position.x + (change * dt.asSeconds()), character_position.y};
	bool collided = check_character_collision(next_pos, tiles_in_row, collision_array, interactive_grid, tiles_in_level, character_grid);
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
	//std::cout << character_name << ": velocity_x = " << velocity_x << std::endl; //DEBUG
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
	if (this->character_name == "player")
	{
		if (attack_time >= sf::seconds(0.45f))
		{
			meele_attack_state = MeeleAttackState::None;
			kick_attack_state = KickAttackState::None;
			attack_time = sf::seconds(0.f);
		}
	}
	else if (this->character_name == "enemy")
	{
		if (attack_time >= sf::seconds(0.75f))
		{
			meele_attack_state = MeeleAttackState::None;
			kick_attack_state = KickAttackState::None;
			attack_time = sf::seconds(0.f);
		}
	}
	else if (this->character_name == "menu_fighter")
	{
		attackbox.size = { 500.f, 100.f };
		attackbox.position = { 1443,340 };
		if (attack_time >= sf::seconds(0.45f))
		{
			meele_attack_state = MeeleAttackState::None;
			kick_attack_state = KickAttackState::None;
		}
		if (attack_time >= sf::seconds(1.5f))
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
	else return 0;
}

Interactive* Character::get_standing_on_platform()
{
	return standing_on_platform;
}

void Character::apply_platform_velocity()
{
	if (standing_on_platform != nullptr)
	{
		character_position.x += get_standing_on_platform()->get_actual_velocity_x();
		character_position.y += get_standing_on_platform()->get_actual_velocity_y();
	}
}

void Character::set_character_position(float pos_x, float pos_y)
{
	character_position = { pos_x, pos_y };
	std::cout << character_sprite.getLocalBounds().size.x << "x" << character_sprite.getLocalBounds().size.y << std::endl;
}

void Character::menu_fighter_fighting(sf::Time& dt)
{
	if (meele_attack_state == MeeleAttackState::None && kick_attack_state == KickAttackState::None && attack_time == sf::seconds(0.f))
	{
		int random = std::rand() % 100;
		if (random < 30)
			meele_attack_state = MeeleAttackState::Attack1;
		else if (random < 60)
			meele_attack_state = MeeleAttackState::Attack3;
		else if (random < 80)
			kick_attack_state = KickAttackState::AttackHigh;
		else
			kick_attack_state = KickAttackState::AttackMiddle;


	}
	attack(dt);

}

void Character::set_attacked(bool at)
{
	attacked = at;
}

bool Character::get_attacked()
{
	return attacked;
}

bool Character::get_attacking_status()
{
	if (meele_attack_state == MeeleAttackState::None && kick_attack_state == KickAttackState::None)
		return 0;
	else return 1;
}

sf::FloatRect& Character::get_character_attackbox()
{
	return attackbox;
}

bool Character::get_character_attackbox_status()
{
	return attackbox_active;
}

std::string& Character::get_character_name()
{
	return character_name;
}

const bool Character::get_character_is_dying() const
{
	return character_is_dying;
}

bool Character::get_right_side() const
{
	return right_side;
}

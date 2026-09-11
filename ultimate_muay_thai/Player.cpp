#include "Player.hpp"
#include "Character.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Interactive.hpp"
#include "Gem.hpp"	
#include <vector>
#include <cstdlib>
#include <SFML/Audio.hpp>
#include "Game_status.hpp"



Player::Player()
{
	is_fighting = false;
	is_moving = false;
	is_blocking = false;
	animation_stage = 0;
	character_name = "player";
	if (!character_texture.loadFromFile("textures/player_textures_new.png"))
		std::cout << "Error loading player texture from file\n";
	character_sprite.setTexture(character_texture);
	character_position = {respawn_position};
	character_sprite.setPosition(character_position);
	character_sprite.setTextureRect(get_frame_position(0));
	hitbox.size = { 64,128 - 128/8};
	hitbox.position = { character_position.x + character_sprite.getLocalBounds().size.x / 2 - hitbox.size.x / 2, character_position.y + character_sprite.getLocalBounds().size.y / 7 };
	right_side = true;
	moving_normal = 0;
	moving_fight = 0;
	is_falling = 0;
	is_jumping = 0;
	on_ground = 0;

	velocity_x = 0;
	
	//HP
	hp = 10;
	max_hp = 10;

	//SPECIAL POINTS
	special_points = 0;
	max_special_points = 20;

	//LIFES
	lifes = MAX_LIFES;

	//knocked
	knocked = 0;
	time_knocked = sf::seconds(0.f);
	transparenting_status = 0;
	transparenting_time = sf::seconds(0.f);

	//moving_block
	moving_block = 0;
	time_moving_block = sf::seconds(0.f);

	//platform
	standing_on_platform = nullptr ;
}

void Player::update_cutscenes(sf::Time& dt, Game_status& status, sf::View& camera)
{
	time_animation += dt;
	if (pistol_mode)
	{
		cutscene_in_progress = true;

		if (camera.getSize().x > 480.f)
			camera.setSize({ camera.getSize().x - 1440.f * dt.asSeconds(), camera.getSize().y });
		if (camera.getSize().y > 270.f)
			camera.setSize({ camera.getSize().x, camera.getSize().y - 810.f * dt.asSeconds() });

		if (devils_head_difference >= DEVILS_HEAD_DIFFERENCE_MAX)
			devils_head_up = false;
		else if (devils_head_difference <= 0.f)
			devils_head_up = true;
		if(devils_head_up)
		{
			devils_head_difference += 10.f * dt.asSeconds();
			devils_head_sprite.move({ 0.f, -10.f * dt.asSeconds() });
		}
		else
		{
			devils_head_difference -= 10.f * dt.asSeconds();
			devils_head_sprite.move({ 0.f, 10.f * dt.asSeconds() });
		}


		if (pistol_mode_cutscene_music.getStatus() != sf::Music::Status::Playing)
			pistol_mode_cutscene_music.play();
		if (time_animation <= sf::seconds(5.259f))
			pistol_mode_cutscene_music.setPan(0.8f);
		else pistol_mode_cutscene_music.setPan(0.f);
		if (time_animation <= sf::seconds(8.74f))
			character_sprite.setTextureRect(get_frame_position(37));
		else if (time_animation <= sf::seconds(10.833f))
			character_sprite.setTextureRect(get_frame_position(38));
		else character_sprite.setTextureRect(get_frame_position(39));
		if (time_animation >= sf::seconds(11.609f))
		{
			status = Game_status::Running;
			pistol_mode_time = sf::seconds(0.f);
			camera.setSize({ 1920,1080 });
			cutscene_in_progress = false;
			pistol_mode_cutscene_music.stop();
		}
	}
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

	if (player_dead)
	{
		if (animation_stage == 0)
			character_sprite.setTextureRect(get_frame_position(26));
		else if (animation_stage == 1)
			character_sprite.setTextureRect(get_frame_position(27));
		else if(animation_stage == 2) 
			character_sprite.setTextureRect(get_frame_position(28));
	}
	else if(attacked)
		character_sprite.setTextureRect(get_frame_position(26));
	else if (is_shooting_animation)
	{
		if (animation_stage == 0) character_sprite.setTextureRect(get_frame_position(31));
		else if (animation_stage == 1) character_sprite.setTextureRect(get_frame_position(32));
		
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
				if (!pistol_mode)
					character_sprite.setTextureRect(get_frame_position(5));
				else
					character_sprite.setTextureRect(get_frame_position(33));
			else if (animation_stage == 1)
				if (!pistol_mode)
					character_sprite.setTextureRect(get_frame_position(6));
				else
					character_sprite.setTextureRect(get_frame_position(34));
			else if (animation_stage == 2)
				if (!pistol_mode)
					character_sprite.setTextureRect(get_frame_position(7));
				else
					character_sprite.setTextureRect(get_frame_position(35));
		}
		else if(on_ground)
		{
			if (animation_stage == 0)
				if (!pistol_mode)
					character_sprite.setTextureRect(get_frame_position(0));
				else
					character_sprite.setTextureRect(get_frame_position(29));
			else if (animation_stage == 1)
				if (!pistol_mode)
					character_sprite.setTextureRect(get_frame_position(1));
				else character_sprite.setTextureRect(get_frame_position(30));
		}
	}
	if (on_ground)std::cout << "ON_GROUND" << std::endl;
	else std::cout << "NOT ON GROUND\n";
}

void Player::update_frame_status(sf::Time& dt)
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
			if(transparenting_status == 0)
				transparenting_status = 1;
			else transparenting_status = 0;
			transparenting_time = sf::seconds(0.f);
		}
	}
	else transparenting_status = 0;

	if (player_dead)
	{
		if (time_animation < sf::seconds(0.25))
			animation_stage = 0;
		else if (time_animation < sf::seconds(0.60))
			animation_stage = 1;
		else animation_stage = 2;
	}
	else if (is_shooting_animation)
	{
		if (moving_normal > 0) is_shooting_animation = 0;
		else if (time_animation < sf::seconds(0.05f))
			animation_stage = 0;
		else if (time_animation <= sf::seconds(0.20f))
			animation_stage = 1;
		else
		{
			time_animation = sf::seconds(0.f);
			animation_stage = 0;
			is_shooting_animation = false;
		}
	}
	else if (is_jumping) //jumping
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
		else if(meele_attack_state != MeeleAttackState::None)
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
				if(attack_time < sf::seconds(0.15f))
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

void Player::check_player_events(const std::optional<sf::Event>& event, sf::Time& dt, Game_status& status)
{
	if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->scancode == sf::Keyboard::Scancode::Space && !is_jumping && !is_falling && !pistol_mode)
		{
			if (is_fighting)
				is_fighting = false;
			else is_fighting = true;
		}

		if (pistol_mode)
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::H && !moving_fight && !moving_normal && !is_blocking && !is_falling && !is_jumping
				&& shooting_latency == sf::seconds(0.f))
			{
				is_shooting = true;
				is_shooting_animation = true;
				is_fighting = false;
				animation_stage = 0;
				time_animation = sf::seconds(0.f);
				shooting_latency += dt;
			}
		}
		if (is_fighting)
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::H && moving_fight == 0 && !is_blocking)
			{
				moving_fight = 0;
				starting_strike = true;
				kick_attack_state = KickAttackState::None;
				switch (meele_attack_state)
				{
				case MeeleAttackState::None:
				{
					meele_attack_state = MeeleAttackState::Attack1;
					attack_time = sf::seconds(0.f);
					break;
				}
				case MeeleAttackState::Attack1:
				{
					if (attack_time >= sf::seconds(0.3f))
					{
						meele_attack_state = MeeleAttackState::Attack2;
						attack_time = sf::seconds(0.f);
					}
					break;
				}
				case MeeleAttackState::Attack2:
				{
					if (attack_time >= sf::seconds(0.3f))
					{
						meele_attack_state = MeeleAttackState::Attack3;
						attack_time = sf::seconds(0.f);
					}
					break;
				}
				case MeeleAttackState::Attack3:
				{
					if (attack_time >= sf::seconds(0.3f))
					{
						meele_attack_state = MeeleAttackState::Attack1;
						attack_time = sf::seconds(0.f);
					}
					break;
				}
				default:
					break;
				}
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::J && !is_blocking && moving_fight == 0)
			{
				moving_fight = 0;
				meele_attack_state = MeeleAttackState::None;
				starting_strike = 1;
				if (kick_attack_state == KickAttackState::None)
				{
					if (attack_dir == 0)
					{
						kick_attack_state = KickAttackState::AttackMiddle;
						attack_time = sf::seconds(0.f);
					}
					else if (attack_dir == 1)
					{
						kick_attack_state = KickAttackState::AttackHigh;
						attack_time = sf::seconds(0.f);
					}
					else if (attack_dir == 2)
					{
						kick_attack_state = KickAttackState::AttackLow;
						attack_time = sf::seconds(0.f);
					}
				}
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::Y)
			{
				if (right_side) right_side = 0;
				else right_side = 1;
			}
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::W && !is_falling && !is_fighting && !is_jumping)
		{
				is_jumping = 1;
				on_ground = 0;
				velocity_y = -1000.f;
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::U && special_points == max_special_points && !is_jumping && !is_falling && !pistol_mode)
		{
			pistol_mode = true;
			status = Game_status::Cutscene;
			devils_head_sprite.setPosition({ character_position.x + 80.f, character_position.y + 10.f });
			time_animation = sf::seconds(0.f);
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::Numpad1)
		{
			hp--;
			if (hp < 0) hp = 0;
		}
		else if (keyPressed->scancode == sf::Keyboard::Scancode::Numpad2)
		{
			hp++;
			if (hp > max_hp) hp = max_hp;
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::Numpad4)
		{
			special_points--;
			if (special_points < 0) special_points = 0;
		}
		else if (keyPressed->scancode == sf::Keyboard::Scancode::Numpad5)
		{
			special_points++;
			if (special_points > max_special_points) special_points = max_special_points;
		}
	}
}

void Player::check_player_collisions_with_interactive(const int tiles_in_row, const std::vector<std::vector<Interactive*>>& interactive_grid, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid)	
{

	standing_on_platform = nullptr;
	sf::FloatRect checking_rect = { hitbox.position, hitbox.size };
	int left = checking_rect.position.x / 128.f;
	int right = (checking_rect.position.x + hitbox.size.x) / 128.f;
	int top = checking_rect.position.y / 128.f;
	int bottom = (checking_rect.position.y + hitbox.size.y) / 128.f;


	for(int y = top; y <= bottom + 1; y++) //more tiles are checking
		for (int x = left; x <= right; x++)
		{
			if (x < 0 || y < 0 || x >= tiles_in_row || y >= tiles_in_level / tiles_in_row)
			{
				continue;
			}
			else
			{
				short int index = x + y * tiles_in_row;
				for (int i = 0; i < interactive_grid[index].size(); i++)
				{
					if (checking_rect.findIntersection(interactive_grid[index][i]->get_object_sprite().getGlobalBounds()) && interactive_grid[index][i]->get_status())
					{
						if (interactive_grid[index][i]->get_object_type() == "hp_gem" && hp < max_hp) //HP GEM
						{
							hp++;
							interactive_grid[index][i]->set_destroyed(1);
						}
						else if (interactive_grid[index][i]->get_object_type() == "special_gem" && special_points < max_special_points) //SPECIAL GEM
						{
							special_points++;
							interactive_grid[index][i]->set_destroyed(1);
						}
						else if (interactive_grid[index][i]->get_object_type() == "spiked_roller" ) //SPIKED ROLLER
						{
							if (checking_rect.position.x < interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.x)
							{
								if (!knocked)
									velocity_x = -600.f;
								else velocity_x = -300.f;
							}
							else if(checking_rect.position.x + checking_rect.size.x > interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.x + interactive_grid[index][i]->get_object_sprite().getGlobalBounds().size.x)
							{
								if (!knocked)
									velocity_x = 600.f;
								else velocity_x = 300.f;
							}

							if (checking_rect.position.y < interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.y)
								if (!knocked)
									velocity_y = -600.f;
								else velocity_y = -300.f;
							else if(checking_rect.position.y + checking_rect.size.y > interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.y + interactive_grid[index][i]->get_object_sprite().getGlobalBounds().size.y)
								if (!knocked)
									velocity_y = 600.f;
								else velocity_y = 300.f;

							on_ground = 0;
							moving_block = 1;
							if (!knocked)
							{
								hp--;
								knocked = 1;
							}
							
						}
						else if (interactive_grid[index][i]->get_object_type() == "spikes") //SPIKES
						{
							int dir = interactive_grid[index][i]->get_direction();
							sf::FloatRect obj = interactive_grid[index][i]->get_object_sprite().getGlobalBounds();
							std::cout << "DIR: " << dir << std::endl;
							switch (dir)
							{
							case 1:
								if (!knocked)
									velocity_y = 600.f;
								else velocity_y = 10.f;
								if (checking_rect.position.x < obj.position.x)
									if (!knocked)
										velocity_x = -600.f;
									else velocity_x = -10.f;
								else if (checking_rect.position.x + checking_rect.size.x > obj.position.x + obj.size.x)
									if (!knocked)
										velocity_x = 600.f;
									else velocity_x = 10.f;
								break;
							case 2:
									velocity_y = -600.f;
								if (checking_rect.position.x < obj.position.x)
									if (!knocked)
										velocity_x = -600.f;
									else velocity_x = -10.f;
								else if (checking_rect.position.x + checking_rect.size.x > obj.position.x + obj.size.x)
									if (!knocked)
										velocity_y = 600.f;
									else velocity_y = 10.f;
								break;
							case 3:
								velocity_x = 600.f;
								if (checking_rect.position.y < obj.position.y)
									velocity_y = -600.f;
								else if (checking_rect.position.y + checking_rect.size.y > obj.position.y + obj.size.y)
									velocity_y = 600.f;
								break;
							case 4:
								velocity_x = -600.f;
								if (checking_rect.position.y < obj.position.y)
									velocity_y = -600.f;
								else if (checking_rect.position.y + checking_rect.size.y > obj.position.y + obj.size.y)
									velocity_y = 600.f;
								break;
							default:
								if (!knocked)
									velocity_y = 600.f;
								else velocity_y = 10.f;
								if (checking_rect.position.x < obj.position.x)
									if (!knocked)
										velocity_x = -600.f;
									else velocity_x = -10.f;
								else if (checking_rect.position.x + checking_rect.size.x > obj.position.x + obj.size.x)
									if (!knocked)
										velocity_x = 600.f;
									else velocity_x = 10.f;
							}
							moving_block = 1;
							if (!knocked)
							{
								knocked = 1;
								hp--;
							}
							
						}

					}
					
					if (checking_rect.findIntersection(interactive_grid[index][i]->get_checkpoint_rect()) && interactive_grid[index][i]->get_object_type() == "checkpoint" && interactive_grid[index][i]->get_status()) // CHECKPOINT
					{
						interactive_grid[index][i]->set_status(false);
						interactive_grid[index][i]->set_checkpoint_is_drawing();
						respawn_position = interactive_grid[index][i]->get_checkpoint_rect().position;
					}
					else if (checking_rect.findIntersection(interactive_grid[index][i]->get_checkpoint_rect()) && interactive_grid[index][i]->get_object_type() == "you_win")
					{
						player_win = true;
					}



					if (interactive_grid[index][i]->get_object_type() == "moving_tile") //MOVING TILE
					{
						if (checking_rect.position.x < interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.x + interactive_grid[index][i]->get_object_sprite().getGlobalBounds().size.x
							&& checking_rect.position.x + checking_rect.size.x >= interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.x
							&& std::abs(interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.y - (checking_rect.position.y + checking_rect.size.y)) <= 10.f
							&& !is_jumping)
						{
							standing_on_platform = interactive_grid[index][i];
							on_ground = 1;
							is_falling = 0;
							character_position.y = interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.y - character_sprite.getGlobalBounds().size.y;
						}
						else if (checking_rect.position.x < interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.x
							&& checking_rect.findIntersection(interactive_grid[index][i]->get_object_sprite().getGlobalBounds())
							&& interactive_grid[index][i]->get_direction() == 2)
							character_position.x -= std::abs((checking_rect.position.x + checking_rect.size.x) - interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.x);

						else if (checking_rect.position.x + checking_rect.size.x > interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.x + interactive_grid[index][i]->get_object_sprite().getGlobalBounds().size.x
							&& checking_rect.findIntersection(interactive_grid[index][i]->get_object_sprite().getGlobalBounds())
							&& interactive_grid[index][i]->get_direction() == 1)
								character_position.x += std::abs(checking_rect.position.x - (interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.x + interactive_grid[index][i]->get_object_sprite().getGlobalBounds().size.x));

						else if (checking_rect.position.y + checking_rect.size.y > interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.y + interactive_grid[index][i]->get_object_sprite().getGlobalBounds().size.y
							&& checking_rect.findIntersection(interactive_grid[index][i]->get_object_sprite().getGlobalBounds())
							&& interactive_grid[index][i]->get_direction() == 4)
						{
							character_position.y += std::abs(checking_rect.position.y - (interactive_grid[index][i]->get_object_sprite().getGlobalBounds().position.y + interactive_grid[index][i]->get_object_sprite().getGlobalBounds().size.y)) + 10.f;
							velocity_y = 0.f;
						}
					}


					if (attackbox.findIntersection(interactive_grid[index][i]->get_object_sprite().getGlobalBounds()) && attackbox_active && interactive_grid[index][i]->get_punched() == 0 && interactive_grid[index][i]->get_status() && !interactive_grid[index][i]->get_blocking_status())
					{
						if (interactive_grid[index][i]->get_object_type() == "punching_bag") //PUNCHING BAG
						{
							//std::cout << interactive_objects[index]->get_hp() << std::endl;
							interactive_grid[index][i]->decrease_hp(get_damage());
							if (right_side)
								interactive_grid[index][i]->set_punched(1);
							else if (!right_side)
								interactive_grid[index][i]->set_punched(2);
							//std::cout << right_side << std::endl;
							if (interactive_grid[index][i]->get_hp() <= 0)
								interactive_grid[index][i]->set_destroyed(true);
						}
						else if (interactive_grid[index][i]->get_object_type() == "enemy") //ENEMY
						{
								interactive_grid[index][i]->decrease_hp(get_damage());
								attackbox_active = false;
								interactive_grid[index][i]->set_punched(1);
								if (interactive_grid[index][i]->get_hp() <= 0)
									interactive_grid[index][i]->set_is_dying(true);
						}
					}

				}
				for (int i = 0; i < character_grid[index].size(); i++)
				{
					if (checking_rect.findIntersection(character_grid[index][i]->get_character_attackbox()) && character_grid[index][i]->get_character_attackbox_status() && !attacked && !is_blocking && character_grid[index][i] != this)
						{
							hp -= character_grid[index][i]->get_damage();
							attacked = true;
							attackbox_active = false;
						}
				}
				
			}
		}
	//if (standing_on_platform != nullptr) std::cout << "ON platform!\n";  //DEBUG
	//else std::cout << "NOT platform\n";
	}	

void Player::check_pressed()
{
	if (is_fighting && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl))
	{
		is_blocking = 1;
		moving_fight = 0;
		moving_normal = 0;
		attackbox_active = 0;
	}
	else is_blocking = 0;

	if (!is_fighting)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) && !moving_block)
		{
			moving_normal = 2;
			moving_fight = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) && !moving_block)
		{
			moving_normal = 1;
			moving_fight = 0;
		}
		else moving_normal = 0;
	}
	else if(is_fighting && !is_falling && !is_jumping && !is_blocking)
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
		{
			attack_dir = 1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
		{
			attack_dir = 2;
		}
		else attack_dir = 0;
	}

}

sf::Vector2f& Player::get_player_position()
{
	return character_position;
}

sf::Vector2f Player::get_player_center()
{
	return sf::Vector2f{ hitbox.position.x + (hitbox.size.x / 2), hitbox.position.y + (hitbox.size.y / 2) };
}

const short int Player::get_special_points()
{
	return special_points;
}

const short int Player::get_max_special_points()
{
	return max_special_points;
}

void Player::knocked_moving_latency(sf::Time& dt)
{
	if (knocked)
	{
		time_knocked += dt;
		if (time_knocked >= sf::seconds(2.f))
		{
			knocked = 0;
			time_knocked = sf::seconds(0.f);
		}
	}
	if (moving_block)
	{
		time_moving_block += dt;
		if (time_moving_block >= sf::seconds(0.2f))
		{
			moving_block = 0;
			time_moving_block = sf::seconds(0.f);
		}
	}
}

const bool Player::get_knocked()
{
	return knocked;
}

void Player::character_position_update()
{
	character_sprite.setPosition(character_position);
	hitbox.position = { character_position.x + character_sprite.getLocalBounds().size.x / 2 - hitbox.size.x / 2, character_position.y + character_sprite.getLocalBounds().size.y / 8 };
}

const bool& Player::get_starting_strike()
{
	return starting_strike;
}

void Player::set_starting_strike(const bool flag)
{
	starting_strike = flag;
}

void Player::check_attacked(sf::Time& dt)
{
	if (attacked_time >= sf::seconds(0.35f))
	{
		attacked = false;
		attacked_time = sf::seconds(0.f);
	}
		else attacked_time += dt;
}

void Player::pistol_mode_check(sf::Time& dt)
{
	pistol_mode_time += dt;
	if (pistol_mode_time >= sf::seconds(1.f))
	{
		pistol_mode_time = sf::seconds(0.f);
		special_points--;
	}

	if (shooting_latency > sf::seconds(0.f) && shooting_latency <= sf::seconds(0.5f))
	{
		shooting_latency += dt;
	}
	else shooting_latency = sf::seconds(0.f);

	if (special_points <= 0)
	{
		pistol_mode = false;
		shooting_latency = sf::seconds(0.f);
	}


}

const bool& Player::get_pistol_mode() const
{
	return pistol_mode;
}

const bool& Player::get_is_shooting() const
{
	return is_shooting;
}

void Player::set_is_shooting(const bool flag)
{
	is_shooting = flag;
}

const sf::Vector2u& Player::get_player_size() const
{
	return character_texture.getSize();
}

void Player::set_hp_to_default()
{
	hp = max_hp;
}

void Player::set_special_to_default()
{
	special_points = 0;
}

void Player::set_player_dead()
{
	player_dead = 1;
}

const bool& Player::get_player_dead() const
{
	return player_dead;
}

void Player::check_hp(std::unique_ptr<Main_menu>& main_menu, Game_status& game_status, short int& current_level, std::unique_ptr<Level>& level)
{
	if (hp <= 0)
		set_player_dead();
	if (player_dead && time_animation >= sf::seconds(2.f) && lifes > 0)
		respawn();
	else if (player_dead && lifes == 0)
	{
		game_status = Game_status::Game_over;
		main_menu->game_over_menu_set_up();
		main_menu->set_menu_status(Main_menu::Menu_status::Game_over_menu);
		if (time_animation >= sf::seconds(5.f))
		{
			main_menu->main_menu_set_up();
			current_level = 0;
			level = nullptr;
			set_hp_to_default(); set_special_to_default(); set_lifes_to_default();
			player_dead = false;
			time_animation = sf::seconds(0.f);
			main_menu->set_menu_status(Main_menu::Menu_status::Main_menu);
			game_status = Game_status::Main_menu;
		}		
	}
}

void Player::respawn()
{
	hp = max_hp;
	lifes--;
	player_dead = false;
	character_position = respawn_position;
	knocked = true;
}

void Player::set_start_respawn(sf::Vector2f pos)
{
	std::cout << "POS_X: " << pos.x << " POS_Y: " << pos.y << std::endl;
	respawn_position = pos;
	character_position = pos;
	character_sprite.setPosition(pos);
}

const short int Player::get_lifes() const
{
	return lifes;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &character_texture;
	target.draw(character_sprite, states);
	if (pistol_mode && cutscene_in_progress)
	{
		target.draw(devils_head_sprite, states);
	}
}

void Player::set_lifes_to_default()
{
	lifes = MAX_LIFES;
}

const bool& Player::get_player_win() const
{
	return player_win;
}

void Player::set_player_win(bool w)
{
	player_win = w;
}
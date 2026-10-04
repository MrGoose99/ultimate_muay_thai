#pragma once
#include <SFML/Graphics.hpp>
#include "Character.hpp"
#include "HUD.hpp"
#include <array>
#include <vector>
#include "Interactive.hpp"
#include "Game_status.hpp"
#include <SFML/Audio.hpp>
#include "Level.hpp"
#include "Main_menu.hpp"
#include "Game_status.hpp"

class Player : public Character
{
protected:

	
	//flags//////////////////////////////////////////////////////////////////////////////////////

	//boolean variable to check if the player is blocking or not
	bool attack_flag = { 0 };
	int attack_dir = { 0 }; //0 - middle, 1 high, 2 - low

	//SPECIAL
	short int special_points;
	short int max_special_points;

	short int lifes;
	const short int MAX_LIFES = { 3 };

	bool starting_strike = { 0 };

	bool is_shooting = { 0 };

	bool is_shooting_animation = { 0 };

	//moving_block
	bool moving_block;
	sf::Time time_moving_block;

	//
	bool pistol_mode = {false};
	sf::Time pistol_mode_time = { sf::seconds(0.f) };
	sf::Time shooting_latency = { sf::seconds(0.f) };

	sf::Time player_dying_time = { sf::seconds(0.f) };

	sf::Time attack_latency = { sf::seconds(0.5f) };
	
	sf::Vector2f respawn_position = { 0,0 };

	bool player_dead = { false };
	bool player_game_over = { false };

	bool player_win = { false };

	//PISTOL_MODE_CUTSCENE
	sf::Texture devils_head_texture{ "textures/devils_head.png" };
	sf::Sprite devils_head_sprite{ devils_head_texture };
	float devils_head_difference = { 0.f };
	float DEVILS_HEAD_DIFFERENCE_MAX = { 10.f };
	bool devils_head_up = { true };
	bool cutscene_in_progress = { false };

	//AUDIO
		//player_sounds
	sf::Music pistol_mode_cutscene_music{ "sound/effects/bulletproof.wav" };
	sf::SoundBuffer gunshot_soundbuffer{ "sound/effects/gunshot.wav" };

	sf::Sound gunshot_sound{ gunshot_soundbuffer };
	sf::SoundBuffer jump_soundbuffer{ "sound/effects/jump.wav" };
	sf::Sound jump_sound{ jump_soundbuffer };

	sf::SoundBuffer running_soundbuffer{ "sound/effects/running.wav" };
	sf::Sound running_sound{ running_soundbuffer };
	sf::Time running_time = { sf::seconds(0.f) };
	sf::SoundBuffer running_fight_soundbuffer{ "sound/effects/running_fight.wav" };
	sf::Sound running_fight_sound{ running_fight_soundbuffer };

	sf::SoundBuffer huff_punch_soundbuffer{ "sound/effects/punch_huff.wav" };
	sf::Sound huff_punch_sound{ huff_punch_soundbuffer };
	sf::SoundBuffer kick_shout_soundbuffer{ "sound/effects/kick_shout.wav" };
	sf::Sound kick_shout_sound{ kick_shout_soundbuffer };

	sf::SoundBuffer blast_hit_soundbuffer{ "sound/effects/boss_blast_hit.wav" };
	sf::Sound blast_hit_sound{ blast_hit_soundbuffer };

	//interaction_sounds
	sf::SoundBuffer punching_bag_attack_soundbuffer{ "sound/effects/punching_bag_attack.wav" };
	sf::Sound punching_bag_attack_sound{ punching_bag_attack_soundbuffer };

	sf::SoundBuffer enemy_hit_1_soundbuffer{ "sound/effects/enemy_hit_1.wav" };
	sf::SoundBuffer enemy_hit_2_soundbuffer{ "sound/effects/enemy_hit_2.wav" };
	sf::SoundBuffer enemy_hit_dead_soundbuffer{ "sound/effects/enemy_hit_dead.wav" };
	sf::Sound enemy_hit_sounds[3] = { sf::Sound{ enemy_hit_1_soundbuffer }, sf::Sound{ enemy_hit_2_soundbuffer }, sf::Sound{ enemy_hit_dead_soundbuffer } };
	sf::SoundBuffer block_hit_soundbuffer{ "sound/effects/block_hit.wav" };
	sf::Sound block_hit_sound{ block_hit_soundbuffer };

	sf::SoundBuffer player_hit_1_soundbuffer{ "sound/effects/player_hit_1.wav" };
	sf::SoundBuffer player_hit_2_soundbuffer{ "sound/effects/player_hit_2.wav" };
	sf::SoundBuffer player_hit_dead_soundbuffer{ "sound/effects/enemy_hit_dead.wav" };
	sf::Sound player_hit_sounds[3] = { sf::Sound{ player_hit_1_soundbuffer }, sf::Sound{ player_hit_2_soundbuffer }, sf::Sound{ player_hit_dead_soundbuffer } };

	sf::SoundBuffer spike_hurt_soundbuffer{ "sound/effects/spike_hurt.wav" };
	sf::Sound spike_hurt_sound{ spike_hurt_soundbuffer };

	sf::SoundBuffer win_soundbuffer{ "sound/effects/win_sound.wav" };
	sf::Sound win_sound{ win_soundbuffer };

	sf::SoundBuffer checkpoint_soundbuffer{ "sound/effects/checkpoint_sound.wav" };
	sf::Sound checkpoint_sound{ checkpoint_soundbuffer };

	sf::SoundBuffer game_over_soundbuffer{ "sound/effects/game_over_sound.wav" };
	sf::Sound game_over_sound{ game_over_soundbuffer };

	sf::SoundBuffer gem_soundbuffer{ "sound/effects/gem_sound.wav" };
	sf::Sound gem_sound{ gem_soundbuffer };

public:
	friend class Level;
	friend class HUD;
	
	//constructor//////////////////////////////////////////////////////////////////////////////////////
	Player();

	//position
	void character_position_update();
	
	//animations//////////////////////////////////////////////////////////////////////////////////////
	void update_character_animation(sf::Time& dt) override; //setting the move animation of the character:
										//0 - 1 - default pose normal, 2-3 - default pose fight, 4 - block, 5-7 - moving normal, 8-9 - moving fight
	void update_frame_status(sf::Time& dt) override; //updating the status of the frame (for example, for a 2-frame animation, it will be 0 for the first frame and 1 for the second frame)

	void update_cutscenes(sf::Time& dt, Game_status& status, sf::View& camera);
	
	//checking events//////////////////////////////////////////////////////////////////////////////////////
	void check_player_events(const std::optional<sf::Event>& event, sf::Time& dt, Game_status& status); //checking events related to the player
	void check_pressed(sf::Time& dt); //checking pressed buttons
	void knocked_moving_latency(sf::Time& dt); //change knocked boolean
	void check_attacked(sf::Time& dt);
	void pistol_mode_check(sf::Time& dt);
	void check_hp(std::unique_ptr<Main_menu>& main_menu, Game_status& game_status, short int& current_level, std::unique_ptr<Level>& level, sf::Music& music);

	void respawn();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;



	//collisions//////////////////////////////////////////////////////////////////////////////////////
	void check_player_collisions_with_interactive(const int tiles_in_row, const std::vector<std::vector<Interactive*>>& interactive_grid, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid); //checking collisions of the player with interactive objects (for example, with a health pack)
	
	//getters//////////////////////////////////////////////////////////////////////////////////////
	bool get_is_fighting() const; //returning the fighting status of the player
	bool get_is_moving() const; //returning the moving status of the player
	bool get_is_blocking() const; //returning the blocking status of the player
	sf::Vector2f& get_player_position();
	sf::Vector2f get_player_center();
	const short int get_special_points();
	const short int get_max_special_points();
	const bool get_knocked();
	Interactive* get_standing_on_platform();
	const bool& get_starting_strike();
	const bool& get_pistol_mode() const;
	const bool& get_is_shooting() const;
	const sf::Vector2u& get_player_size() const;
	const bool& get_player_dead() const;
	const short int get_lifes() const;
	const bool& get_player_win() const;


	//setters//////////////////////////////////////////////////////////////////////////////////////
	void set_is_fighting(const bool status); //setting the fighting status of the player
	void set_is_moving(const bool status); //setting the moving status of the player
	void set_is_blocking(const bool status); //setting the blocking status of the player
	void set_starting_strike(const bool flag);
	void set_is_shooting(const bool flag);
	void set_hp_to_default();
	void set_special_to_default();
	void set_player_dead();
	void set_start_respawn(sf::Vector2f pos);
	void set_lifes_to_default();
	void set_player_win(bool w);
	void set_attack_latency_to_zero();
	void set_meele_attack_state_to_none();

};
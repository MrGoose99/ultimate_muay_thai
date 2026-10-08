#pragma once
#include "Character.hpp"
#include "Interactive.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Boss : public Character, public Interactive
{
protected:
	enum class BossState
	{
		None,
		Walking,
		Rush,
		Blast,
		CloseApproach,
		Unconscious,
		Dying
	};
	BossState boss_state = { BossState::None };

	sf::Texture aura_texture{ "textures/power_aura.png" };
	sf::Sprite aura_sprite{ aura_texture };

	std::string type = { "boss" };
	std::string name = {type};

	//timers
	sf::Time walking_time = { sf::seconds(0.f) };
	sf::Time rushing_time = { sf::seconds(0.f) };
	sf::Time blasting_time = { sf::seconds(0.f) };
	sf::Time unconscious_time = { sf::seconds(0.f) };
	sf::Time boss_attack_time = { sf::seconds(0.f) };
	sf::Time boss_time_to_attack = { sf::seconds(0.f) };

	sf::Time boss_blasting_animation_time = { sf::seconds(0.f) };
	sf::Time boss_aura_time = { sf::seconds(0.f) };

	//Rush
	sf::Vector2f starting_rushing_position;
	float rushing_distance = { 0.f };
	bool rushing_attack_done = { false }; //variable that protect player from taking damage * FPS


	//Aura
	bool aura_active = { true };
	int aura_stage = { 0 };

	//Blast
	int blasting_stage = { 0 };

	int random_attack = { 0 }; //Blast or Rush are randomizing, 0 - none, 1 - Rush, 2 - Blast
	int attacks = { 0 }; //how many attacks are used before Unconscious state

	//Dying
	float dying_transparenting = { 255.f }; //to dissapear

	//Audio
	sf::SoundBuffer boss_blast_soundbuffer{ "sound/effects/boss_blast_shot.wav" };
	sf::Sound boss_blast_sound{ boss_blast_soundbuffer };

	sf::SoundBuffer boss_panting_soundbuffer{ "sound/effects/boss_panting.wav" };
	sf::Sound boss_panting_sound{ boss_panting_soundbuffer };

	sf::SoundBuffer boss_shout_1_soundbuffer{ "sound/effects/boss_shout_1.wav" };
	sf::SoundBuffer boss_shout_2_soundbuffer{ "sound/effects/boss_shout_2.wav" };
	sf::SoundBuffer boss_shout_3_soundbuffer{ "sound/effects/boss_shout_3.wav" };
	sf::Sound boss_shout_sounds[3] = { sf::Sound{ boss_shout_1_soundbuffer }, sf::Sound{ boss_shout_2_soundbuffer }, sf::Sound{ boss_shout_3_soundbuffer } };


public:

	Boss(int starting, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::vector<Character*>>& character_grid, int tiles_in_row);
	void update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid) override;
	void texture_update(sf::Time& dt) override;
	void update_character_animation(sf::Time& dt) override;
	void update_frame_status(sf::Time& dt) override;
	void state_update(Player& p1);
	void grid_update(std::vector<std::vector<Interactive*>>& interactive_grid, std::vector<std::vector<Character*>>& character_grid, int tiles_in_row);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	//getters
	std::string get_object_type() override;
	const sf::Sprite& get_object_sprite() const override;

};
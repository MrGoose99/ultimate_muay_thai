#pragma once
#include "Character.hpp"
#include "Interactive.hpp"
#include <SFML/Graphics.hpp>

class Boss : public Character, public Interactive
{
protected:
	enum class BossState
	{
		None,
		Walking,
		Rush,
		Blast,
		Attack,
		Unconscious,
		Dying
	};
	BossState boss_state = { BossState::None };

	std::string type = { "boss" };
	std::string name = {type};

	//timers
	sf::Time walking_time = { sf::seconds(0.f) };
	sf::Time rushing_time = { sf::seconds(0.f) };
	sf::Time blasting_time = { sf::seconds(0.f) };
	sf::Time unconscious_time = { sf::seconds(0.f) };
	sf::Time boss_attack_time = { sf::seconds(0.f) };

	//Rush
	sf::Vector2f starting_rushing_position;
	float rushing_distance = { 0.f };
	bool rushing_attack_done = { false }; //variable that protect player from taking damage * FPS
	bool boss_hitbox_active = { false }; //when boss rushing, he detects attack on player by his hitbox

	//Blast
	bool blasting = { false };
	int blasting_stage = 0;

	int random_attack = { 0 }; //Blast or Rush are randomizing, 0 - none, 1 - Rush, 2 - Blast
	int attacks = { 0 }; //how many attacks are used before Unconscious state

	bool immortality = { true }; //boss can take damage only when he is unconscious

	//Dying
	float dying_transparenting = 255.f; //to dissapear





public:

	Boss(int starting, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::vector<Character*>>& character_grid, int tiles_in_row);
	void update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid) override;
	void texture_update(sf::Time& dt) override;
	void update_character_animation(sf::Time& dt) override;
	void update_frame_status(sf::Time& dt) override;
	void state_update(Player& p1);
	void grid_update(std::vector<std::vector<Interactive*>>& interacitve_grid, std::vector<std::vector<Character*>>& character_grid, int tiles_in_row);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	//getters
	std::string get_object_type() override;
	const sf::Sprite& get_object_sprite() const override;

};
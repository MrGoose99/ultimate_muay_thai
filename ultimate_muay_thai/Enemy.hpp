#pragma once
#include <SFML/Graphics.hpp>
#include "Character.hpp"
#include "Interactive.hpp"
#include "Player.hpp"

class Enemy : public Character, public Interactive
{
protected:
	enum class EnemyState //state of enemy in that moment
	{
		None, //doing nothing, far away from player
		Patroling, //walking and looking for player
		Rushing, //rushing at player, front to player
		Attacking, //attacking player
		Blocking //blocking
	};
	EnemyState enemy_state;
	std::string type = { "enemy" };
	int patrol_tile;
	bool patroling_direction = { 1 };
	
public:
	Enemy(int starting, std::vector<std::vector<Interactive*>>& moving_objects, int tiles_in_row);
	void update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level) override;
	void texture_update(sf::Time& dt) override;
	void update_character_animation(sf::Time& dt) override;
	void update_frame_status(sf::Time& dt) override;
	void state_update(Player& p1);
	void interactive_grid_update(std::vector<std::vector<Interactive*>>& interacitve_grid, int tiles_in_row);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//getters
	std::string get_object_type() override;
	const sf::Sprite& get_object_sprite() const override;
};


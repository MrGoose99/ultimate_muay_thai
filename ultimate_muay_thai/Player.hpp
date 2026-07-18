#pragma once
#include <SFML/Graphics.hpp>
#include "Character.hpp"
#include "HUD.hpp"
#include <array>
#include <vector>
#include "Interactive.hpp"

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



	//moving_block
	bool moving_block;
	sf::Time time_moving_block;



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
	
	//checking events//////////////////////////////////////////////////////////////////////////////////////
	void check_player_events(const std::optional<sf::Event>& event, sf::Time& dt); //checking events related to the player
	void check_pressed(); //checking pressed buttons
	void knocked_moving_latency(sf::Time& dt); //change knocked boolean


	//collisions//////////////////////////////////////////////////////////////////////////////////////
	void check_player_collisions_with_interactive(const int tiles_in_row, std::vector<std::vector<Interactive*>> interactive_grid, const int& tiles_in_level); //checking collisions of the player with interactive objects (for example, with a health pack)
	
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

	//setters//////////////////////////////////////////////////////////////////////////////////////
	void set_is_fighting(const bool status); //setting the fighting status of the player
	void set_is_moving(const bool status); //setting the moving status of the player
	void set_is_blocking(const bool status); //setting the blocking status of the player
};
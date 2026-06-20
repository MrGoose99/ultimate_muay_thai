#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "Interactive.hpp"

class Character : public sf::Drawable, public sf::Transformable
{
protected:
	//texture//////////////////////////////////////////////////////////////////////////////////////
	sf::Texture character_texture; //texture of the character
	std::string character_name; //name of the character

	//animation//////////////////////////////////////////////////////////////////////////////////////
	sf::Time time_animation = sf::seconds(0.f); //time variable to control the animation of the character
	sf::Sprite character_sprite{ character_texture }; //sprite of the character

	//stats//////////////////////////////////////////////////////////////////////////////////////
	short int hp; //health points
	short int max_hp; //maximum health points

	//moving//////////////////////////////////////////////////////////////////////////////////////
	int moving_normal; //is character moving: 0 - not moving, 1 - moving left, 2 - moving right
	int moving_fight; //is character moving when fight: 0 - not moving, 1 - moving left, 2 - moving right
	sf::FloatRect hitbox; //hitbox of the character
	sf::Vector2f character_position;
	float jump_actual_high = { 0 }; //run jump high, if reach jump_high value, jumping ends
	const float gravity = { 1800.f }; //gravitation
	const float max_fall_speed = { 800.f }; //maximum speed of falling
	float velocity_y = { 0.f }; //up/down moving
	float velocity_x = { 0.f }; //left/right moving for special occasion

	//attack////////////////////////////////////////////////////////////////////////////////////////
	sf::FloatRect attackbox; //attack zone
		
	bool attackbox_active = { 0 }; //is attackbox_active
	sf::Time attack_time = { sf::seconds(0.f) }; //time to change attack_stage
	enum class MeeleAttackState
	{
		None,
		Attack1,
		Attack2,
		Attack3
	};
	MeeleAttackState meele_attack_state = MeeleAttackState::None;
	enum class KickAttackState
	{
		None,
		AttackHigh,
		AttackMiddle,
		AttackLow
	};
	KickAttackState kick_attack_state = KickAttackState::None;
	//flags//////////////////////////////////////////////////////////////////////////////////////
	bool right_side; //is character_sprite looking on the right?
	bool is_fighting; //boolean variable to check if the player is in fighting mode or not
	bool is_falling; //boolean variable to check if the player is falling
	bool is_moving; //boolean variable to check if the player is moving
	bool is_jumping = { 0 }; //checking is on air and going up
	bool on_ground; //checking is on ground

	//debugging//////////////////////////////////////////////////////////////////////////////////////
	sf::RectangleShape debug; //debugging Rectangle (hitbox)
	sf::RectangleShape debug2; //debugging Rectangle (hitbox)
	
public:
	
	//animation//////////////////////////////////////////////////////////////////////////////////////
	virtual void update_character_animation(sf::Time& dt) = 0; //setting the move animation of the character
	virtual void update_frame_status(sf::Time& dt) = 0; //updating the status of the frame (for example, for a 2-frame animation, it will be 0 for the first frame and 1 for the second frame)
	
	//drawing//////////////////////////////////////////////////////////////////////////////////////
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override; //drawing the character

	//attack//////////////////////////////////////////////////////////////////////////////////////
	void attack(sf::Time& dt);

	//moving//////////////////////////////////////////////////////////////////////////////////////
	void character_moving(sf::Time& dt, const float speed_normal, const float speed_fight, std::vector<bool>& collision_array, std::vector<std::vector<Interactive*>>& interactive_grid, short int tiles_in_row);
	const unsigned int get_tile_number(unsigned int tiles_in_row); //getting number of tile in tilemap (tile = 128x128)
	void character_position_update(); //updating position of sprite and hitbox
	bool check_character_collision(const sf::Vector2f position, const int tiles_in_row, std::vector<bool>& collision_array, std::vector<std::vector<Interactive*>>& interactive_grid); //checking is position collide
	void check_velocity_y(sf::Time& dt, std::vector<bool>& collision_array, const int tiles_in_row, std::vector<std::vector<Interactive*>>& interactive_grid);
	void check_velocity_x(sf::Time& dt, std::vector<bool>& collision_array, std::vector<std::vector<Interactive*>>& interactive_grid);
	
	//platforms
	bool on_platform = { 0 };
	Interactive* current_platform;

	//debug//////////////////////////////////////////////////////////////////////////////////////
	sf::RectangleShape get_debug_shape();
	sf::RectangleShape get_debug_2_shape();

	//getters//////////////////////////////////////////////////////////////////////////////////////
	sf::IntRect get_frame_position(short int frame_number); //returning the position of the frame in the texture based on the frame number and total frames in the animation
	sf::Sprite& get_character_sprite(); //returning the character sprite
	sf::FloatRect& get_character_hitbox(); //returning the character hitbox
	sf::Vector2f& get_character_position(); //returning the character position
	const short int get_hp(); //returning the health points of the character
	const short int get_max_hp(); //returning the maximum health points of the character
	const short int get_damage()&;
	
	//setters//////////////////////////////////////////////////////////////////////////////////////
	void set_character(std::filesystem::path& texture, std::string& char_name, sf::Vector2f& pos); //setting character texture, name and position

};
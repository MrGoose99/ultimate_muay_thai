#pragma once
#include <SFML/Graphics.hpp>

class Character;
class Player;
class Interactive: public sf::Drawable, public sf::Transformable
{
protected:
	sf::FloatRect hitbox;
	sf::Texture object_texture;
	bool status = { 1 };
	int hp = { 5 };
	int tile_number = { 0 };
	short int punched = { 0 };
	sf::Time punched_time = { sf::seconds(0.f) };
	std::string type = { "" };

	//dynamic tile position
	int spawning_tile;
	int current_tile;
	std::vector<int> actual_tiles;

	//for platform
	float actual_velocity_x;
	float actual_velocity_y;
	short int direction; //1 - right, 2 - left, 3 - up, 4 - down
	//
	bool is_destroyed = { false };
	std::vector<std::vector<Interactive*>> interactive_grid;

	bool is_blocking = { 0 };

	//for checkpoint
	sf::FloatRect checkpoint_rect = { {0,0},{0,0} };
	bool checkpoint_is_drawing;

	//dying
	sf::Time dying_time = { sf::seconds(0.f) };
	bool is_dying = { false };
	bool by_bullet = { false };

	//to turning enemy into gem
	bool gem_spawned = { false };

public:
	void virtual update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid) = 0; //updating the status of the interactive object (position, interactive)
	void virtual texture_update(sf::Time& dt) = 0;
	void update_punched(sf::Time& dt);

	//getters
	virtual std::string get_object_type() = 0; //getting the type of the interactive object (for example, "gem", "heart", etc.)
	virtual const sf::Sprite& get_object_sprite() const = 0; //getting the sprite of the interactive object
	bool get_status();
	int get_hp()&;
	const short int get_tile_number()&;
	const short int get_punched()&;
	const short int get_current_tile()&;
	bool get_destroyed();
	float get_actual_velocity_x();
	float get_actual_velocity_y();
	short int get_direction();
	std::vector<int>& get_actual_tiles();
	bool get_blocking_status();
	const bool get_is_dying() const;
	const bool& get_gem_spawned() const;
	const sf::FloatRect get_checkpoint_rect() const;
	const bool get_checkpoint_is_drawing() const;

	//setters
	void set_status(bool stat);
	void decrease_hp(short int points);
	void set_punched(short int p);
	void set_current_tile(const int current);
	void set_destroyed(bool des);
	void set_is_dying(const bool flag);
	void set_by_bullet(const bool flag);
	void set_gem_spawned(bool flag);
	void set_checkpoint_is_drawing();


};
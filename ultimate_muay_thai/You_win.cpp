#include "You_win.hpp"
#include "Game_status.hpp"
#include "Main_menu.hpp"

You_win::You_win(float x, float y, int tile, int tiles_in_row)
{
	float position_x = tile % tiles_in_row * 128.f;
	float position_y = tile / tiles_in_row * 128.f;
	checkpoint_rect = { {position_x, position_y}, {128.f, 128.f} };
	status = 1;
	current_tile = tile;
}

void You_win::update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid)
{
	//empty
}
void You_win::texture_update(sf::Time& dt)
{
	//empty
}
std::string You_win::get_object_type()
{
	return type;
}
const sf::Sprite& You_win::get_object_sprite() const
{
	return you_win_sprite;
}
void You_win::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//empty
}


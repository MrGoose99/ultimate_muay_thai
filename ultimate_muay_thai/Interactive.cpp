#include "Interactive.hpp"
#include <iostream>

void Interactive::set_status(bool stat)
{
	status = stat;
}

bool Interactive::get_status()
{
	return status;
}

int Interactive::get_hp()&
{
	return hp;
}

void Interactive::decrease_hp(short int points)
{
	if(!immortality) hp -= points;
}

const short int Interactive::get_tile_number()&
{
	return tile_number;
}

const short int Interactive::get_punched()&
{
	return punched;
}

void Interactive::set_punched(short int p)
{
	punched = p;
}

void Interactive::update_punched(sf::Time& dt)
{
	punched_time += dt;
	if (punched_time >= sf::seconds(0.2f))
	{
		set_punched(0);
		punched_time = sf::seconds(0.f);
	}
}

const short int Interactive::get_current_tile()&
{
	return current_tile;
}

void Interactive::set_current_tile(const int current)
{
	current_tile = current;
}

void Interactive::set_destroyed(bool des)
{
	is_destroyed = des;
}

bool Interactive::get_destroyed()
{
	return is_destroyed;
}

float Interactive::get_actual_velocity_x()
{
	return actual_velocity_x;
}

float Interactive::get_actual_velocity_y()
{
	return actual_velocity_y;
}

short int Interactive::get_direction()
{
	return direction;
}

std::vector<int>& Interactive::get_actual_tiles()
{
	return actual_tiles;
}

bool Interactive::get_blocking_status()
{
	return is_blocking;
}

const bool Interactive::get_is_dying() const
{
	return is_dying;
}

void Interactive::set_is_dying(const bool flag)
{
	is_dying = flag;
}

void Interactive::set_by_bullet(const bool flag)
{
	by_bullet = flag;
}

const bool& Interactive::get_gem_spawned() const
{
	return gem_spawned;
}

void Interactive::set_gem_spawned(bool flag)
{
	gem_spawned = flag;
}

const sf::FloatRect Interactive::get_checkpoint_rect() const
{
	return checkpoint_rect;
}

void Interactive::set_checkpoint_is_drawing()
{
	checkpoint_is_drawing = true;
}

const bool Interactive::get_checkpoint_is_drawing() const
{
	return checkpoint_is_drawing;
}

void Interactive::set_boss_blasting(bool flag)
{
	blasting = flag;
}

bool Interactive::get_boss_blasting()
{
	return blasting;
}

bool Interactive::get_boss_immortality()
{
	return immortality;
}




#include "Interactive.hpp"

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
	hp -= points;
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
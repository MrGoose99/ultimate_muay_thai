#include "Checkpoint.hpp"
#include <iostream>

Checkpoint::Checkpoint(float x, float y, int tile, int tiles_in_row)
{
	float position_x = tile % tiles_in_row * 128.f;
	float position_y = tile / tiles_in_row * 128.f;
	checkpoint_rect = { {position_x, position_y}, {128.f, 128.f} };
	status = 1;
	current_tile = tile;

	checkpoint_text.setString("CHECKPOINT");
	checkpoint_text.setFillColor(sf::Color::White);
	checkpoint_text.setCharacterSize(20);
	checkpoint_text.setOrigin({ checkpoint_text.getLocalBounds().size.x / 2, checkpoint_text.getLocalBounds().size.y / 2 });
	checkpoint_text.setPosition({ position_x + 64.f, position_y + 20.f });

	checkpoint_text_shadow.setString("CHECKPOINT");
	checkpoint_text_shadow.setFillColor(sf::Color::Black);
	checkpoint_text_shadow.setCharacterSize(20);
	checkpoint_text_shadow.setOrigin({ checkpoint_text_shadow.getLocalBounds().size.x / 2, checkpoint_text_shadow.getLocalBounds().size.y / 2 });
	checkpoint_text_shadow.setPosition({ position_x + 69.f, position_y + 25.f });

	checkpoint_is_drawing = false;
}

void Checkpoint::update(sf::Time& dt, float moving_speed, std::vector<std::vector<Interactive*>>& moving_objects, std::vector<std::unique_ptr<Interactive>>& interactive_objects, short int tiles_in_row, Player& p1, std::vector<bool>& collision_array, const int& tiles_in_level, std::vector<std::vector<Character*>>& character_grid)
{
	if (checkpoint_is_drawing)
	{
		drawing_time += dt;
		checkpoint_text.move({ 0.f, -moving_speed * dt.asSeconds() });
		//checkpoint_text_shadow.setPosition({ checkpoint_text.getPosition().x + 5.f, checkpoint_text.getPosition().y + 5.f });
		checkpoint_text_shadow.move({ 0.f, -moving_speed * dt.asSeconds() });
		if (drawing_time >= sf::seconds(0.7f))
		{
			alpha -= 255.f * dt.asSeconds();
			checkpoint_text.setFillColor(sf::Color(255, 255, 255, alpha));
			checkpoint_text_shadow.setFillColor(sf::Color(0, 0, 0, alpha));
		}
		if (alpha <= 0)
		{
			checkpoint_is_drawing = false;
			is_destroyed = true;
		}
	}
}
void Checkpoint::texture_update(sf::Time& dt)
{
	//empty
}
std::string Checkpoint::get_object_type()
{
	return type;
}
const sf::Sprite& Checkpoint::get_object_sprite() const
{
	return checkpoint_sprite;
}

void Checkpoint::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (checkpoint_is_drawing)
	{
		target.draw(checkpoint_text_shadow, states);
		target.draw(checkpoint_text, states);
	}
}
#pragma once

#include <SFML/Graphics.hpp>

class TileMap :public sf::Drawable, public sf::Transformable
{
protected:
	sf::VertexArray m_vertices; //array of verticles - base to render the tilemap
	sf::Texture m_tileset; //member of class, textures of tileset

public:
	bool set_tilemap(const std::filesystem::path& tileset, sf::Vector2u tile_size, const int* tiles, unsigned int width, unsigned int height);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override; //drawing the tilemap
};
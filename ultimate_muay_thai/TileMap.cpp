#include <SFML/Graphics.hpp>
#include "TileMap.hpp"
#include <iostream>
#include <cmath>

bool TileMap::set_tilemap(const std::filesystem::path& tileset, sf::Vector2u tile_size, const int* tiles, unsigned int width, unsigned int height)
{

	m_height = height; //setting height of the tilemap
	m_width = width; //setting width of the tilemap

	if (!m_tileset.loadFromFile(tileset)) //error control and loading the tileset
	{
		std::cout << "Error loading tileset" << std::endl;
		return false;
	}

	m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles); //setting vertices as triangles
	m_vertices.resize(width * height * 6); //setting size of vertices array (6 vertices for each tile)

	for (unsigned int i = 0; i < width; ++i) //setting tiles positions and texture coordinates
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			const int tileNumber = tiles[i + j * width]; //number of tile in the tileset

			const int tile_x = tileNumber % (m_tileset.getSize().x / tile_size.x); //x coordinate of the tile in the tileset
			const int tile_y = tileNumber / (m_tileset.getSize().x / tile_size.x); //y coordinate of the tile in the tileset

			sf::Vertex* triangles = &m_vertices[(i + j * width) * 6]; //pointer to the current tile's vertices

			//setting positions of vertices
			triangles[0].position = sf::Vector2f(i * tile_size.x, j * tile_size.y);
			triangles[1].position = sf::Vector2f((i + 1) * tile_size.x, j * tile_size.y);
			triangles[2].position = sf::Vector2f(i * tile_size.x, (j + 1) * tile_size.y);
			triangles[3].position = sf::Vector2f(i * tile_size.x, (j + 1) * tile_size.y);
			triangles[4].position = sf::Vector2f((i + 1) * tile_size.x, j * tile_size.y);
			triangles[5].position = sf::Vector2f((i + 1) * tile_size.x, (j + 1) * tile_size.y);

			//setting texture coordinates of vertices
			triangles[0].texCoords = sf::Vector2f(tile_x * tile_size.x, tile_y * tile_size.y);
			triangles[1].texCoords = sf::Vector2f((tile_x + 1) * tile_size.x, tile_y * tile_size.y);
			triangles[2].texCoords = sf::Vector2f(tile_x * tile_size.x, (tile_y + 1) * tile_size.y);
			triangles[3].texCoords = sf::Vector2f(tile_x * tile_size.x, (tile_y + 1) * tile_size.y);
			triangles[4].texCoords = sf::Vector2f((tile_x + 1) * tile_size.x, tile_y * tile_size.y);
			triangles[5].texCoords = sf::Vector2f((tile_x + 1) * tile_size.x, (tile_y + 1) * tile_size.y);
		}
	}
	return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform(); //applying the transform of the tilemap

	states.texture = &m_tileset; //applying the tileset texture

	target.draw(m_vertices, states); //drawing the tilemap
}

void TileMap::draw_culled(sf::RenderTarget& target, sf::RenderStates states, const sf::View& camera) const
{
	states.transform *= getTransform();
	states.texture = &m_tileset;

	constexpr float tile_size = 128.f;
	constexpr float additional_tiles = 2 * 128.f;

	sf::Vector2f center = camera.getCenter();
	sf::Vector2f size = camera.getSize();

	float left = center.x - size.x / 2.f;
	float right = center.x + size.x / 2.f;
	float top = center.y - size.y / 2.f;
	float bottom = center.y + size.y / 2.f;

	int start_x = static_cast<int>(left / tile_size) - additional_tiles;
	int end_x = static_cast<int>(right / tile_size) + additional_tiles;

	int start_y = static_cast<int>(top / tile_size) - additional_tiles;
	int end_y = static_cast<int>(bottom / tile_size) + additional_tiles;

	start_x = std::max(0, start_x);
	start_y = std::max(0, start_y);
	
	end_x = std::min(static_cast<int>(m_width) - 1, end_x);
	end_y = std::min(static_cast<int>(m_height) - 1, end_y);

	for (int y = start_y; y <= end_y; ++y)
	{
		for (int x = start_x; x <= end_x; ++x)
		{
			int index = (x + y * m_width);

			const sf::Vertex* triangles = &m_vertices[index * 6];

			target.draw(triangles, 6, sf::PrimitiveType::Triangles, states);
		}
	}


}



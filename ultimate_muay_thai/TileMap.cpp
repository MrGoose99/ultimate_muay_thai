#include <SFML/Graphics.hpp>
#include "TileMap.hpp"
#include <iostream>

bool TileMap::set_tilemap(const std::filesystem::path& tileset, sf::Vector2u tile_size, const int* tiles, unsigned int width, unsigned int height)
{
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



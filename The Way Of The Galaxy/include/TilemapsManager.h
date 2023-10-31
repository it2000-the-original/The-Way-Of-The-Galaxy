#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Point.h"
#include "Polygon.h"

class Entity;

struct Tile {

	int id;
	SDL_Rect source;

	bool hasCollider = false;
	std::size_t colliderId;
	Polygon polygon;
};

struct Tileset {

	SDL_Texture* texture;

	int tileWidth;
	int tileHeight;

	int columns;
	int lines;

	std::vector<Tile> tiles;
};

using Tilesets = std::map<std::string, Tileset>;

struct TileMap {

	std::string tileset;

	std::vector<std::vector<int>> map; // the matrix of the csv file
};

using TileMaps = std::map<std::string, TileMap>;

class TilemapsManager {

private:

	Tilesets tilesets;
	TileMaps tilemaps;

	const std::string mapsPrefix = "tilemaps/";
	const std::string tilesetsPrefix = mapsPrefix + "tilesets/";

	bool loading;

	int column;
	double speed;
	double position;
	TileMap* loadingTilemap;
	Tileset* loadingTileset;

	const int chunk = 1;

public:

	TilemapsManager();

	void update();

	void addTileset(std::string patch);
	void addTilesets(std::string patch);
	void addMaps(std::string patch);
	void addMap(nlohmann::json map);

	void loadMap(std::string name, double speed);
	void loadMap(std::string name, double speed, int _column);
};

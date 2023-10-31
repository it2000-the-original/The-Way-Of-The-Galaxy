#include <nlohmann/json.hpp>
#include "TilemapsManager.h"
#include "TextureManager.h"
#include "ECS/Components.h"
#include "Engine.h"
#include "ECS.h"

using namespace nlohmann;

void addTilesProperties(Tileset& tileset, json tiles);
void addTiles(Tileset& tileset);

TilemapsManager::TilemapsManager() {}

void TilemapsManager::update() {
	
	if (loading and column < loadingTilemap->map[0].size()) {

		if (position - renderwidth <= loadingTileset->tileWidth * chunk) {

			// Define the position of a new column
			position += loadingTileset->tileWidth;

			for (int i = 0; i < loadingTilemap->map.size(); i++) {

				int tileID = loadingTilemap->map[i][column];

				// -1 is interpreted as void space

				if (tileID != -1) {

					auto& entity = Engine::manager.addEntity();

					Rectangle space = {

						position,
						i * loadingTileset->tileHeight + statusheight,
						loadingTileset->tileWidth,
						loadingTileset->tileHeight
					};

					Tile tile = loadingTileset->tiles[tileID];

					entity.addComponent<PositionComponent>(space, 1);
					entity.addComponent<SpriteComponent>(loadingTileset->texture);
					entity.addComponent<AutodestroyComponent>(lx);

					if (tile.hasCollider and !tile.polygon.empty())
						entity.addComponent<ColliderComponent>(tile.colliderId, tile.polygon);
					else if (tile.hasCollider)
						entity.addComponent<ColliderComponent>(tile.colliderId);

					entity.addGroup(wallId);

					entity.getComponent<SpriteComponent>().setSource(tile.source);
					entity.getComponent<PositionComponent>().setSpeed(speed, 0);
				}
			}

			column++;
		}

		position += speed; // Update the position of the last column
	}

	else {

		loading = false;
	}
}

void TilemapsManager::addTileset(std::string patch) {
	
	json data;
	Tileset tileset;

	std::ifstream file(patch);

	if (file.fail()) {

		// The file does not exist
		std::cout << "Tileset Error: the file \"" << patch << "\" does not exist!" << std::endl;
		return;
	}

	try {

		data = json::parse(file);

		std::string name = data["name"].get<std::string>();

		int textureWidth = data["imagewidth"].get<int>();
		int textureHeight = data["imageheight"].get<int>();

		tileset.texture = TextureManager::LoadTexture((tilesetsPrefix + data["image"].get<std::string>()).c_str());
		tileset.tileWidth = data["tilewidth"].get<int>();
		tileset.tileHeight = data["tileheight"].get<int>();
		tileset.columns = data["columns"].get<int>();
		tileset.lines = textureHeight / (textureWidth / tileset.columns);

		addTiles(tileset);

		addTilesProperties(tileset, data["tiles"]);

		file.close();

		tilesets.emplace(name, tileset);

		std::cout << "Added tileset: \"" << patch << "\"" << std::endl;
	}

	catch (json::parse_error& ex) {

		// The file is corrupted
		std::cout << "Tileset Error: the file \"" << patch << "\" is corrupted! " << ex.what() << std::endl;
		file.close();
	}

	catch (json::type_error& ex) {

		// The file has one or more values errors
		std::cout << "Tileset Error: invalid properties in \"" << patch << "\" " << ex.what() << std::endl;
		file.close();
	}
}

void TilemapsManager::addTilesets(std::string patch) {

	json data;

	std::ifstream file(patch);

	if (file.fail()) {

		// The file does not exist
		std::cout << "Tilesets Error: the file \"" << patch << "\" does not exist!" << std::endl;
		return;
	}

	try {

		data = json::parse(file);

		for (json tilepatch : data["tilesetsList"])
			addTileset(mapsPrefix + tilepatch.get<std::string>());

		file.close();

		std::cout << "Added tilesets from: \"" << patch << "\"" << std::endl;
	}

	catch (json::parse_error& ex) {

		// The file is corrupted, error reading the file
		std::cout << "Tilesets Error: the file \"" << patch << "\" is corrupted! " << ex.what() << std::endl;
		file.close();
	}

	catch (json::type_error& ex) {

		// Wrong values inside the file
		std::cout << "Tilesets Error: invalid properties in \"" << patch << "\" " << ex.what() << std::endl;
		file.close();
	}
}

void TilemapsManager::addMaps(std::string patch) {

	json data;

	std::ifstream file(patch);

	if (file.fail()) {

		// The file does not exist
		std::cout << "Tilemaps Error: the file \"" << patch << "\" does not exist!" << std::endl;
		return;
	}

	try {

		data = json::parse(file);

		for (json map : data["tilemapsList"]) addMap(map);

		file.close();

		std::cout << "Added maps from: \"" << patch << "\"" << std::endl;
	}

	catch (json::parse_error& ex) {
		
		// The file is corrupted
		std::cout << "Maps Error: the file \"" << patch << "\" is corrupted! " << ex.what() << std::endl;
		file.close();
	}

	catch (json::type_error& ex) {

		// Wrong values in the file
		std::cout << "Maps Error: wrong values inside \"" << patch << "\" " << ex.what() << std::endl;
		file.close();
	}
}

void TilemapsManager::addMap(json map) {

	TileMap tilemap;

	try {

		std::string name = map["name"].get<std::string>();
		tilemap.tileset = map["tileset"].get<std::string>();

		if (tilesets.find(tilemap.tileset) == tilesets.end()) {
			
			// The tileset does not exist
			std::cout << "Map Error: the tileset \"" << tilemap.tileset << "\" does not exist!" << std::endl;
			return;
		}

		std::string csvpatch = map["CSV"].get<std::string>();
		std::ifstream csv(csvpatch);

		if (csv.is_open()) {

			std::string line;

			while (std::getline(csv, line)) {

				std::vector<int> row;
				std::stringstream linestream(line);
				std::string cell;

				while (std::getline(linestream, cell, ',')) {

					row.push_back(std::stoi(cell));
				}

				tilemap.map.push_back(row);
			}
		}

		else {

			std::cout << "Map Error: the file \"" << csvpatch << "\" does not exist!" << std::endl;
			return;
		}

		tilemaps.emplace(name, tilemap);

		std::cout << "Added map: \"" << name << "\"" << std::endl;
	}

	catch (json::type_error& ex) {

		// Wrong values in the map
		std::cout << "Map Error: invalid properties in the map file! "  << ex.what() << std::endl;
	}
}

void TilemapsManager::loadMap(std::string name, double _speed) {

	loadingTilemap = &tilemaps[name];
	loadingTileset = &tilesets[loadingTilemap->tileset];
	position = renderwidth + loadingTileset->tileWidth * (chunk - 1) - _speed;
	speed = _speed;
	loading = true;
	column = 0;
}

void TilemapsManager::loadMap(std::string name, double _speed, int _column) {

	loadingTilemap = &tilemaps[name];
	loadingTileset = &tilesets[loadingTilemap->tileset];
	speed = _speed;

	if (_column < loadingTilemap->map[0].size()) {

		position = renderwidth + loadingTileset->tileWidth * (chunk - 1);
		column = _column;
		loading = true;
	}

	else {

		// To prevent an exeption with the map matrix
		column = loadingTilemap->map[0].size() - 1;
		position = renderwidth - loadingTileset->tileWidth * (_column - column + 1);
	}

	for (int i = column; i >= 0; i--)
	for (int j = 0; j < loadingTilemap->map.size(); j++) {

		int tileID = loadingTilemap->map[j][i];

		// -1 is interpreted as void space

		double _position = position - loadingTileset->tileWidth * (column - i - 1);

		if (_position <= -loadingTileset->tileWidth) break;

		if (tileID != -1) {

			auto& entity = Engine::manager.addEntity();

			Rectangle space = {

				_position,
				j * loadingTileset->tileHeight + statusheight,
				loadingTileset->tileWidth,
				loadingTileset->tileHeight
			};

			Tile tile = loadingTileset->tiles[tileID];

			entity.addComponent<PositionComponent>(space, 1);
			entity.addComponent<SpriteComponent>(loadingTileset->texture);
			entity.addComponent<AutodestroyComponent>(lx);

			if (tile.hasCollider and !tile.polygon.empty())
				entity.addComponent<ColliderComponent>(tile.colliderId, tile.polygon);
			else if (tile.hasCollider)
				entity.addComponent<ColliderComponent>(tile.colliderId);

			entity.addGroup(wallId);

			entity.getComponent<SpriteComponent>().setSource(tile.source);
			entity.getComponent<PositionComponent>().setSpeed(speed, 0);
		}
	}
}

void addTiles(Tileset& tileset) {

	for (int i = 0; i < tileset.columns * tileset.lines; i++) {

		Tile tile;

		tile.id = i;
		tile.source.w = tileset.tileWidth;
		tile.source.h = tileset.tileHeight;
		tile.source.x = tileset.tileWidth * (i % tileset.columns);
		tile.source.y = tileset.tileHeight * (i / tileset.columns);

		tileset.tiles.push_back(tile);
	}
}

void addTilesProperties(Tileset& tileset, json tiles) {

	for (json _tile : tiles) {

		Tile& tile = tileset.tiles[_tile["id"].get<int>()];

		if (_tile.contains("properties"))
			for (json property : _tile["properties"])
				if (property["name"].get<std::string>() == "ColliderID") {

					tile.colliderId = property["value"].get<int>();
					tile.hasCollider = true;
				}

		if (_tile["objectgroup"]["objects"][0].contains("polygon")) {

			double x = _tile["objectgroup"]["objects"][0]["x"].get<double>();
			double y = _tile["objectgroup"]["objects"][0]["y"].get<double>();

			for (json point : _tile["objectgroup"]["objects"][0]["polygon"]) {

				tile.polygon.push_back(Point(
					round(point["x"].get<double>() + x),
					round(point["y"].get<double>() + y)
				));
			}
		}
	}
}

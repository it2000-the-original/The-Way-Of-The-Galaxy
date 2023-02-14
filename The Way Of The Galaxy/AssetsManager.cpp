#include "AssetsManager.h"

using namespace nlohmann;

void addAnimations(json data, Asset& asset);
void addPolygon(json data, Asset& asset);
void loadAnimations(Asset asset, Entity& entity);
void loadPolygon(Asset asset, Entity& entity);

AssetsManager::AssetsManager(Manager& mManager) : manager(mManager) {}

Asset AssetsManager::addAsset(std::string name, std::string path) {

	Asset asset;
	json data;

	std::ifstream file(path);

	if (file.fail()) {

		// The file does not exist
		std::cerr << "Error: the file " << path << " does not exist" << std::endl;
		return asset;
	}
	
	try {

		data = json::parse(file);

		// Loading all values in the asset object
		asset.texture = TextureManager::LoadTexture(data["texture"].get<std::string>().c_str());
		asset.width = data["width"].get<int>();
		asset.height = data["height"].get<int>();
		asset.colliderId = data["colliderId"].get<int>();
		asset.groupId = data["renderGroup"].get<int>();
	}

	catch (json::parse_error& ex) {

		// Found errors in the json file
		std::cerr << "Error: invalid json file " << ex.what() << std::endl;
		return asset;
	}

	catch (json::type_error& ex) {

		// Problems with the json values to load
		std::cerr << "Error: invalid or inexistent value " << ex.what() << std::endl;
	}

	addAnimations(data, asset);
	addPolygon(data, asset);
	
	assets.emplace(name, asset);

	return asset;
}

Asset AssetsManager::getAsset(std::string name) {
	
	return assets[name];
}

Entity& AssetsManager::loadAsset(std::string name) {

	Asset asset = getAsset(name);

	SDL_Rect entityRect = { 0, 0, asset.width, asset.height };

	auto& entity = manager.addEntity();

	entity.addComponent<PositionComponent>(entityRect, 1);
	entity.addComponent<SpriteComponent>(asset.texture);
	entity.addComponent<ColliderComponent>(asset.colliderId);
	entity.addGroup(asset.groupId);

	loadAnimations(asset, entity);
	loadPolygon(asset, entity);

	return entity;
}

Entity& AssetsManager::loadAsset(std::string name, int x, int y) {

	Asset asset = getAsset(name);

	SDL_Rect entityRect = { x, y, asset.width, asset.height };

	auto& entity = manager.addEntity();

	entity.addComponent<PositionComponent>(entityRect, 1);
	entity.addComponent<SpriteComponent>(asset.texture);
	entity.addComponent<ColliderComponent>(asset.colliderId);
	entity.addGroup(asset.groupId);

	loadAnimations(asset, entity);
	loadPolygon(asset, entity);

	return entity;
}

////////////////////////////////////////////////////////// External functions......

void addAnimations(json data, Asset& asset) {
	
	// This function load all animations defined
	// in the json file to the asset object

	if (data.contains("animations")) {

		for (auto animation : data["animations"]) {

			try {

				AssetAnimation assetAnimation;

				// Load all animation values
				assetAnimation.name = animation["name"].get<std::string>();
				assetAnimation.index = animation["index"].get<int>();
				assetAnimation.frames = animation["frames"].get<int>();
				assetAnimation.speed = animation["speed"].get<int>();

				asset.animations.push_back(assetAnimation);
			}

			catch (json::type_error& ex) {

				// Found a problem with a json value of the animation
				std::cerr << "Animation Error: invalid or inexistent value " << ex.what() << std::endl;
			}
		}
	}
}

void addPolygon(json data, Asset& asset) {

	// This function load (if existent) a collider
	// polygon defined in the json file in the asset object

	if (data.contains("polygon")) {

		for (auto mConvex : data["polygon"]) {

			Convex convex;

			for (auto mPoint : mConvex) {

				try {

					Point point;

					// Loading point values
					point.x = mPoint["x"].get<float>();
					point.y = mPoint["y"].get<float>();

					if (mPoint.contains("internal"))
						point.internal = mPoint["internal"].get<bool>();

					else point.internal = false;

					convex.push_back(point);
				}

				catch (json::type_error& ex) {

					// Found a problem with the values of the point
					std::cerr << "Polygon Error: invalid or inexistent value " << ex.what() << std::endl;
				}
			}

			asset.polygon.push_back(convex);
		}
	}
}

void loadAnimations(Asset asset, Entity& entity) {

	// This function add all animations in the
	// asset object to the entity in the SpriteComponent

	for (auto animation : asset.animations) {

		entity.getComponent<SpriteComponent>().addAnimation(
			animation.name,
			animation.frames,
			animation.index,
			animation.speed
		);
	}
}

void loadPolygon(Asset asset, Entity& entity) {

	// This function add the polygon of the
	// asset object to the entity in the ColliderComponent

	if (!asset.polygon.empty()) {

		entity.getComponent<ColliderComponent>().setPolygon(asset.polygon);
	}
}

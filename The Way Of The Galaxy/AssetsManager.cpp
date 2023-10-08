#include "AssetsManager.h"
#include "TextureManager.h"
#include "ECS/custom/CustomComponents.h"

using namespace nlohmann;

void addAnimations(json data, Asset& asset);
void addCollider(json data, Asset& asset);
void addPolygon(json data, Asset& asset);
void addTransform(json data, Asset& asset);
void addAutodestroy(json data, Asset& asset);
void addExplosion(json data, Asset& asset);
void addComponent(json data, Asset& asset);

void loadAnimations(Asset asset, Entity& entity);
void loadCollider(Asset asset, Entity& entity);
void loadPolygon(Asset asset, Entity& entity);
void loadTransform(Asset asset, Entity& entity);
void loadAutodestroy(Asset asset, Entity& entity);
void loadExplosion(Asset asset, Entity& entity);
void loadComponent(Asset asset, Entity& entity);

void AssetsManager::addAssets(std::string path) {

	// This function add all assets indicated on a specific file

	json data;

	std::ifstream file(path);

	if (file.fail()) {

		// The file does not exist
		std::cerr << "Assets Error: the file " << path << " does not extst" << std::endl;
		return;
	}

	try {

		data = json::parse(file);
	}

	catch (json::parse_error& ex) {

		// Found errors in the json file
		std::cerr << "Assets Error: invalid json file " << ex.what() << std::endl;
		return;
	}

	if (data.contains("assetsList")) {

		json assetsList = data["assetsList"];

		// Adding all assets defined in the list

		for (auto asset : assetsList) {

			try {

				std::string name = asset["name"].get<std::string>();
				std::string path = asset["path"].get<std::string>();

				addAsset(name, path);
			}

			catch (json::type_error& ex) {

				// Invalid asset values
				std::cerr << "Assets Error: invalid asset values " << ex.what() << std::endl;
			}
		}
	}

	else {

		// Missing argument assetsList
		std::cerr << "Assets Error: missing argument assetsList" << std::endl;
		return;
	}

	std::cout << "Added assets from " << path << " succesfuly" << std::endl;
}

Asset AssetsManager::addAsset(std::string name, std::string path) {

	Asset asset;
	json data;

	std::ifstream file(path);

	if (file.fail()) {

		// The file does not exist
		std::cerr << "Asset Error: the file " << path << " does not exist" << std::endl;
		return asset;
	}
	
	try {

		data = json::parse(file);

		std::string texturepath = data["texture"].get<std::string>();

		// Loading all values in the asset object
		asset.texture = TextureManager::LoadTexture(texturepath.c_str());
		asset.width = data["width"].get<int>();
		asset.height = data["height"].get<int>();
		asset.scale = data["scale"].get<int>();
		asset.groupId = data["renderGroup"].get<std::size_t>();
	}

	catch (json::parse_error& ex) {

		// Found errors in the json file
		std::cerr << "Asset Error: invalid json file " << ex.what() << std::endl;
		return asset;
	}

	catch (json::type_error& ex) {

		// Problems with the json values to load
		std::cerr << "Asset Error: invalid or inexistent value " << ex.what() << std::endl;
		return asset;
	}

	addAnimations(data, asset);
	addCollider(data, asset);
	addPolygon(data, asset);
	addTransform(data, asset);
	addAutodestroy(data, asset);
	addExplosion(data, asset);
	addComponent(data, asset);
	
	assets.emplace(name, asset);

	std::cout << "Added asset " << name << " succesfuly" << std::endl;

	return asset;
}

Asset AssetsManager::getAsset(std::string name) {
	
	return assets[name];
}

Entity& AssetsManager::loadAsset(std::string name) {

	Asset asset = getAsset(name);

	SDL_Rect entityRect = { 0, 0, asset.width, asset.height };

	auto& entity = Engine::manager.addEntity();

	entity.addComponent<PositionComponent>(entityRect, asset.scale);
	entity.addComponent<SpriteComponent>(asset.texture);
	entity.addGroup(int(asset.groupId));

	loadTransform(asset, entity);
	loadAnimations(asset, entity);
	loadCollider(asset, entity);
	loadPolygon(asset, entity);
	loadAutodestroy(asset, entity);
	loadExplosion(asset, entity);
	loadComponent(asset, entity);

	return entity;
}

Entity& AssetsManager::loadAsset(std::string name, int x, int y) {

	Asset asset = getAsset(name);

	SDL_Rect entityRect = { x, y, asset.width, asset.height };

	auto& entity = Engine::manager.addEntity();

	entity.addComponent<PositionComponent>(entityRect, asset.scale);
	entity.addComponent<SpriteComponent>(asset.texture);
	entity.addGroup(int(asset.groupId));

	loadTransform(asset, entity);
	loadAnimations(asset, entity);
	loadCollider(asset, entity);
	loadPolygon(asset, entity);
	loadAutodestroy(asset, entity);
	loadExplosion(asset, entity);
	loadComponent(asset, entity);

	return entity;
}

void addAnimations(json data, Asset& asset) {
	
	// This function load all animations defined
	// in the json file to the asset object

	if (data.contains("animated")) {

		try {

			if (data["animated"].get<bool>()) {

				for (auto animation : data["animations"]) {
				
					AssetAnimation assetAnimation;

					// Load all animation values
					assetAnimation.name = animation["name"].get<std::string>();
					assetAnimation.index = animation["index"].get<int>();
					assetAnimation.frames = animation["frames"].get<int>();
					assetAnimation.speed = animation["speed"].get<int>();

					asset.animations.push_back(assetAnimation);
				}
			}
		}

		catch (json::type_error& ex) {

			// Found a problem with a json value of the animation
			std::cerr << "Animation Error: invalid or inexistent value " << ex.what() << std::endl;
			asset.animations.clear();
		}
	}
}

void addCollider(json data, Asset& asset) {

	// This function define if the asset has a collider

	if (data.contains("hasCollider")) {

		try {

			if (data["hasCollider"].get<bool>()) {

				asset.colliderId = data["colliderId"].get<int>();
				asset.hasCollider = true;
			}
		}

		catch (json::type_error& ex) {

			// Found a problem in the variable
			std::cerr << "Collider Error: invalid or inexistent value " << ex.what() << std::endl;
		}
	}
}

void addPolygon(json data, Asset& asset) {

	// This function load (if existent) a collider
	// polygon defined in the json file in the asset object

	if (data.contains("advancedHitbox")) {

		try {

			if (data["advancedHitbox"].get<bool>()) {

				Polygon polygon;

				for (auto mPoint : data["polygon"]) {

					Point point;

					// Loading point values
					point.x = mPoint["x"].get<float>();
					point.y = mPoint["y"].get<float>();

					if (mPoint.contains("internal"))
						point.internal = mPoint["internal"].get<bool>();

					else point.internal = false;

					polygon.push_back(point);
				}

				asset.polygon = polygon;
			}
		}

		catch (json::type_error& ex) {

			// Found a problem with the values of the point
			std::cerr << "Polygon Error: invalid or inexistent value " << ex.what() << std::endl;
			return;
		}

	}
}

void addExplosion(json data, Asset& asset) {

	// This function add to the asset the
	// possibility to explode to be destroyed

	if (data.contains("explode")) {

		try {

			if (data["explode"].get<bool>()) {

				json explosion = data["explosion"];

				std::string texturepath = explosion["texture"].get<std::string>();

				asset.explosion.texture = TextureManager::LoadTexture(texturepath.c_str());
				asset.explosion.frames = explosion["frames"].get<int>();
				asset.explosion.duration = explosion["duration"].get<int>();
				asset.explosion.width = explosion["width"].get<int>();
				asset.explosion.height = explosion["height"].get<int>();

				asset.explode = true;
			}
		}

		catch (json::type_error& ex) {

			// Found an invalid value of a variable
			std::cerr << "Explosion Error: invalid or inexistent value " << ex.what() << std::endl;
		}
	}
}

void addComponent(json data, Asset& asset) {

	// This function check if the asset must
	// have a custom component and witch

	if (data.contains("hasComponent")) {

		try {

			if (data["hasComponent"].get<bool>()) {

				asset.componentId = data["component"].get<std::size_t>();
				asset.hasComponent = true;
			}
		}

		catch (json::type_error& ex) {

			// Found an invalid value of the variable
			std::cerr << "Component Error: invalid value of componentId " << ex.what() << std::endl;
		}
	}
}

void addAutodestroy(json data, Asset& asset) {

	// This function set the autodestroy
	// variable in the json file

	if (data.contains("autodestroy")) {

		try {

			asset.autodestroy = data["autodestroy"].get<bool>();
		}

		catch (json::type_error& ex) {

			// Found a proble in the variable
			std::cerr << "Controlled Error: invalid value of \"autodestroy\" " << ex.what() << std::endl;
		}
	}
}

void addTransform(json data, Asset& asset) {

	// This function check if the asset
	// must be controlled or not

	if (data.contains("transform")) {

		try {

			asset.transform = data["transform"].get<bool>();
		}

		catch (json::type_error& ex) {

			// Found a problem in the variable
			std::cerr << "Transform Error: invalid value of \"transform\" " << ex.what() << std::endl;
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

void loadCollider(Asset asset, Entity& entity) {

	// This function add the ColliderComponent

	if (asset.hasCollider) {

		entity.addComponent<ColliderComponent>(asset.colliderId);
	}
}

void loadPolygon(Asset asset, Entity& entity) {

	// This function add the polygon of the
	// asset object to the entity in the ColliderComponent

	if (asset.hasCollider and !asset.polygon.empty()) {

		entity.getComponent<ColliderComponent>().setPolygon(asset.polygon);
	}
}

void loadTransform(Asset asset, Entity& entity) {

	// This function add the TransformComponent to the entity
	// if the transform variable is true

	if (asset.transform) {

		entity.addComponent<TransformComponent>();
	}
}

void loadAutodestroy(Asset asset, Entity& entity) {

	// This function add AutodestroyComponent to the entity

	if (asset.autodestroy) {

		entity.addComponent<AutodestroyComponent>();
	}
}

void loadExplosion(Asset asset, Entity& entity) {

	if (asset.explode) {

		entity.addComponent<ExplodeComponent>(asset.explosion);
	}
}
void loadComponent(Asset asset, Entity& entity) {

	// This function add a custom component
	// indicated by a componentId value in the asset

	if (asset.hasComponent) {

		switch (asset.componentId) {

		case playerSpaceship: entity.addComponent<PlayerSpaceship>();  break;
		case missile:         entity.addComponent<MissileComponent>(); break;

		default: std::cerr << "Component Error: Inexistent component id \"" << asset.componentId << "\"" << std::endl;

		}
	}
}
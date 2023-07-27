#include <map>
#include <fstream>
#include <SDL2/SDL.h>
#include <nlohmann/json.hpp>
#include "Explosion.h"
#include "Animation.h"
#include "Polygon.h"
#include "ECS.h"

struct Asset;
struct AssetAnimation;

using Assets = std::map<std::string, Asset>;
using Animations = std::vector<AssetAnimation>;

struct Asset {

	SDL_Texture* texture = nullptr;
	SDL_Texture* explosionTexture = nullptr;

	int width = 0;
	int height = 0;
	int scale = 1;

	std::size_t colliderId = 0;
	std::size_t groupId = 0;
	std::size_t componentId = 0;

	Explosion explosion;
	Animations animations;
	Polygon polygon;

	bool hasComponent = false;
	bool autodestroy = false;
	bool hasCollider = false;
	bool transform = false;
	bool explode = false;
};

struct AssetAnimation {

	std::string name;
	int frames;
	int index;
	int speed;
};

class AssetsManager {

private:

    Assets assets;

public:

    void addAssets(std::string path);
    Asset addAsset(std::string name, std::string path);
    Asset getAsset(std::string name);
    Entity& loadAsset(std::string name);
    Entity& loadAsset(std::string name, int x, int y);
};
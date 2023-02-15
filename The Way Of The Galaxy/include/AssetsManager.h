#include <map>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Components.h"
#include "TextureManager.h"

struct Asset;
struct AssetAnimation;

using Assets = std::map<std::string, Asset>;
using Animations = std::vector<AssetAnimation>;

struct Asset {

    SDL_Texture* texture = nullptr;

    int width = 0;
    int height = 0;

    int colliderId = 0;
    int groupId = 0;

    Animations animations;
    Polygon polygon;
};

struct AssetAnimation {

    std::string name;
    int frames;
    int index;
    int speed;
};

class AssetsManager {

private:

    Manager& manager;
    Assets assets;

public:

    AssetsManager(Manager& mManager);

    Asset addAsset(std::string name, std::string path);
    Asset getAsset(std::string name);
    Entity& loadAsset(std::string name);
    Entity& loadAsset(std::string name, int x, int y);
};
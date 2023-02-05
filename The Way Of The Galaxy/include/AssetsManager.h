#include <map>
#include "TextureManager.h"

struct Asset {

    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
    int colliderId = 0;
    int groupId = 0;
};

class AssetsManager {

private:

    std::map<std::string, Asset*> assets;

public:

    void addAsset(std::string name, std::string path) {}
    Asset* getAsset(std::string name) {}
    void loadAsset(std::string name) {}
    template <typename T> void loadAsset(std::string name);
};
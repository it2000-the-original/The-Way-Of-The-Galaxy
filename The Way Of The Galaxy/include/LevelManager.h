#pragma once
#include <nlohmann/json.hpp>
#include "Components.h"

class LevelManager {

private:

	Manager& manager;
	nlohmann::json levelData;

	int levelBlocksSize;
	int actualBlock;
	bool running;
	Uint32 initialTime;
	Uint32 blockTime;

public:

	LevelManager(Manager& mManager);
	void startLevel(const char* levelPath);
	void spownObject(const char* tex, int position, int width, int height, int speed, int collider, bool reversed);
	bool isRunning();
	void update();
	void stop();
};
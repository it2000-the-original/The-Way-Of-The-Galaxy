#pragma once
#include <string>
#include "ECS/Components.h"
#include "ECS/custom/CustomComponents.h"

class Statusbar;

class Widget {

private:

	int iconSize = 0;
	int iconSpacing = 0;
	bool active = true;

public:

	Entity* entity;
	Statusbar* statusbar;
	TTF_Font* font;

	std::string model = "";
	std::string prefix = "";

	virtual void update() {}

	void setIcon(const char* path, int size, int spacing, int margin);
	void setColor(int r, int g, int b, int a);
	void setPrefix(std::string mPrefix);
	void setModel(std::string mModel);
	void setPosition(int x, int y);

	int getWidth();
	int getIconWidth();

	bool isActive();
	void destroy();
};

///////////////////////////////////////////// Declaring different types of widgets........

class EnergyWidget : public Widget {

	// A widget that show the Health rate
	// of the player

private:

	int* energy;

public:

	EnergyWidget(int* mEnergy);
	void update() override;
};

class MissilesWidget : public Widget {

	// A widget that show the 
	// remaning missiles of the player

private:

	int* missiles;

public:

	MissilesWidget(int* mMissiles);
	void update() override;
};

class WeaponWidget : public Widget {

	// A widget that show the selected
	// weapon by the player

private:

	PlayerSpaceship* player;

public:

	WeaponWidget(PlayerSpaceship* mPlayer);
	void update() override;
};
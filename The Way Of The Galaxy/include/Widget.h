#pragma once
#include "Components.h"
#include <string>

class Statusbar;

class Widget {

private:

	int iconSize = 0;
	int iconSpacing = 0;
	bool active = true;

public:

	Entity* entity;
	Statusbar* statusbar;

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

private:

	int* energy;

public:

	EnergyWidget(int* mEnergy);
	void update() override;
};

class MissilesWidget : public Widget {

private:

	int* missiles;

public:

	MissilesWidget(int* mMissiles);
	void update() override;
};

class WeaponWidget : public Widget {

private:

	PlayerSpaceship* player;

public:

	WeaponWidget(PlayerSpaceship* mPlayer);
	void update() override;
};
#pragma once
#include "Components.h"
#include <string>

struct Status {

	const char* texture;
	const char* font;

	int height = 32;
	int fontSize = 8;
	int marginTop = 0;
	int spacing = 5;
};

class Statusbar;

class Widget {

private:

	std::string model = "";

	int iconSize = 0;
	int iconSpacing = 0;
	bool active = true;

public:

	Entity* entity;
	Statusbar* statusbar;
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

class Statusbar {

private:

	Manager& manager;
	Entity* statusbar;

	int spacing;
	int marginTop;

	const char* fontPath;
	std::vector <Widget*> widgets;

public:

	int fontSize;

	Statusbar(Manager& mManager);

	void init(Status status, bool animated);
	void update();
	void refresh();

	void setAnimation(int f, int i, int s);

	// I cannot define a template function in the header file

	template <typename T, typename... TArgs> T& addWidget(TArgs&&... mArgs) {
		
		T* w = new T(std::forward<TArgs>(mArgs)...);
		widgets.emplace_back(std::move(w));

		auto& e = manager.addEntity();
		e.addComponent<PositionComponent>(0, 0);
		e.addComponent<TextComponent>("none", fontPath, fontSize);
		e.addGroup(groupStatus);

		// Setting dependency pointers

		w->entity = &e;
		w->statusbar = this;

		// Reload the position of all
		// widgets after added the new widget

		reloadPositions();
		return *w;
	}

	void reloadPositions();
};

///////////////////////////////////////////// Defining different types of widgets........

class EnergyWidget : public Widget {

private:

	int* energy;

public:

	EnergyWidget(int* mEnergy) { energy = mEnergy; }

	void update() override { 
		
		entity->getComponent<TextComponent>().setText(prefix + std::to_string(*energy));
	}
};

class MissilesWidget : public Widget {

private:

	int* missiles;

public:

	MissilesWidget(int* mMissiles) { missiles = mMissiles; }

	void update() override {
		
		entity->getComponent<TextComponent>().setText(prefix + std::to_string(*missiles));
	}
};

class WeaponWidget : public Widget {

private:

	PlayerComponent* player;

public:

	WeaponWidget(PlayerComponent* mPlayer) { player = mPlayer; }

	void update() override {
		
		entity->getComponent<TextComponent>().setText(prefix + player->getSelectedWeapon());
	}
};


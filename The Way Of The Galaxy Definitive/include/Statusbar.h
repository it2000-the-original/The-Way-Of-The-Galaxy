#pragma once
#include "Components.h"
#include <string>

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
	void destroy();
	bool isActive();
	int getIconWidth();
	int getWidth();
	void addIcon(const char* iconPath, int mIconSize, int mIconSpacing, int iconMarginTop);
	void setColor(int r, int g, int b);
	void setPrefix(std::string mPrefix);
	void setModel(std::string mModel);
	void setPosition(int x, int y);
	~Widget();
};

class Statusbar {

private:

	int spacing;
	int topPosition;
	Manager& manager;
	Entity* statusbar;
	std::string fontPath;
	std::vector <Widget*> widgets;

public:

	int fontSize;

	Statusbar(Manager& mManager);
	void init(int size, const char* statusTexture, std::string mFontPath, int mFontSize, int mTopPosition, int mSpacing);
	void update();
	void refresh();
	void reloadPositions();

	template <typename T, typename... TArgs> T& addWidget(TArgs&&... mArgs) {
		
		T* w = new T(std::forward<TArgs>(mArgs)...);
		widgets.emplace_back(std::move(w));
		auto& e = manager.addEntity();
		e.addComponent<PositionComponent>(0, 0);
		e.addComponent<TextComponent>("string", fontPath.c_str(), fontSize);
		e.addGroup(groupStatus);
		w->entity = &e;
		w->statusbar = this;
		reloadPositions();
		return *w;
	}
};

///////////////////////////////////////////// Defining different types of widgets........

class EnergyWidget : public Widget {

private:

	int* energy;

public:

	EnergyWidget(int* mEnergy) { energy = mEnergy; }
	void update() override { entity->getComponent<TextComponent>().setText(prefix + std::to_string(*energy)); }
	~EnergyWidget() {}
};

class MissilesWidget : public Widget {

private:

	int* missiles;

public:

	MissilesWidget(int* mMissiles) { missiles = mMissiles; }
	void update() override { entity->getComponent<TextComponent>().setText(prefix + std::to_string(*missiles)); }
	~MissilesWidget() {}
};


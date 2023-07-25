#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Widget.h"
#include "Components.h"

class Statusbar {

private:

	Entity* statusbar;
	SDL_Texture* texture;
	TTF_Font* font = nullptr;

	int width = 1200;
	int height = 30;

	int spacing;
	int marginTop;

	bool active = false;
	bool initializable = false;

	bool animated = false;
	int frames = 0;
	int speed = 0;

	std::vector <Widget*> widgets;

public:

	Statusbar();
	Statusbar(std::string settingsPath);

	void init();
	void update();
	void refresh();

	template <typename T, typename... TArgs> T& addWidget(TArgs&&... mArgs) {

		T* w = new T(std::forward<TArgs>(mArgs)...);

		auto& e = Engine::manager.addEntity();
		e.addComponent<PositionComponent>();
		e.addComponent<TextComponent>("none", font);
		e.addGroup(groupStatus);

		w->statusbar = this;
		w->entity = &e;
		w->font = font;

		if (active) {

			widgets.emplace_back(std::move(w));
			reloadPositions();
		}

		return *w;
	}

	void reloadPositions();
};
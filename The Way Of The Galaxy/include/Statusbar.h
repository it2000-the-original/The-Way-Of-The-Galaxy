#pragma once
#include "Components.h"
#include "Widget.h"
#include <string>

struct Status {

	const char* texture;
	const char* font;

	int height = 32;
	int fontSize = 8;
	int marginTop = 0;
	int spacing = 5;
};

class Statusbar {

private:

	Entity* statusbar;

	int spacing;
	int marginTop;

	const char* fontPath;
	std::vector <Widget*> widgets;

public:

	int fontSize;

	void init(Status status, bool animated);
	void update();
	void refresh();

	void setAnimation(int f, int i, int s);

	// I cannot define a template function in the header file

	template <typename T, typename... TArgs> T& addWidget(TArgs&&... mArgs) {

		T* w = new T(std::forward<TArgs>(mArgs)...);
		widgets.emplace_back(std::move(w));

		auto& e = Engine::manager.addEntity();
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
#include "Statusbar.h"

void Widget::setIcon(const char* path, int size, int spacing, int margin) {

	iconSize = size;
	iconSpacing = spacing;

	Icon icon = { path, size, spacing, margin };

	entity->getComponent<TextComponent>().addIcon(icon, false);
	statusbar->reloadPositions();
}

void Widget::setColor(int r, int g, int b, int a) {

	SDL_Color color = { r, g, b, a };
	entity->getComponent<TextComponent>().setColor(color);
}

void Widget::setPrefix(std::string mPrefix) {

	prefix = mPrefix;
	statusbar->reloadPositions();
}

void Widget::setModel(std::string mModel) {

	model = mModel;
	statusbar->reloadPositions();
}

void Widget::setPosition(int x, int y) {

	entity->getComponent<PositionComponent>().setPosition(x, y);
}

int Widget::getWidth() {

	// Get the width of the widget exluding the icon

	std::string widgetString = prefix + model;
	return widgetString.size() * statusbar->fontSize / 2;
}

int Widget::getIconWidth() {

	// Get the width of the icon
	// including the spacing

	return iconSize + iconSpacing;
}

bool Widget::isActive() { return active; }

void Widget::destroy() { active = false; }

///////////////////////////////////////////////// Statusbar..........

Statusbar::Statusbar(Manager& mManager) : manager(mManager) {}

void Statusbar::init(Status status, bool animated) {

	SDL_Rect statusbarSpace = { 0, 0, 1200, status.height };
	
	statusbar = &manager.addEntity();

	statusbar->addComponent<PositionComponent>(statusbarSpace, 1);
	statusbar->addComponent<SpriteComponent>(status.texture, animated, true);
	statusbar->addGroup(groupStatus);

	fontPath = status.font;
	fontSize = status.fontSize;
	spacing = status.spacing;
	marginTop = status.marginTop;
}

void Statusbar::update() {

	for (auto* widget : widgets) widget->update();
}

void Statusbar::refresh() {

	// Remove all inactive widgets from the vector and from the memory

	widgets.erase(std::remove_if(std::begin(widgets), std::end(widgets), [](Widget* mWidget) {

		if (!mWidget->isActive()) {

			mWidget->entity->destroy();
			delete mWidget;
			return true;
		}

			return false;
	}), 
		
	std::end(widgets));
}

void Statusbar::setAnimation(int f, int i, int s) {

	statusbar->getComponent<SpriteComponent>().addAnimation("statusAnimation", f, i, s);
	statusbar->getComponent<SpriteComponent>().playAnimation("statusAnimation");
}

void Statusbar::reloadPositions() {

	// Move all widgets into the correct position

	int lastPosition = spacing;

	for (auto* widget : widgets) {

		lastPosition += widget->getIconWidth();

		widget->setPosition(lastPosition, marginTop);
		
		lastPosition += widget->getWidth() + spacing;
	}
}
#include "Statusbar.h"

Statusbar::Statusbar(Manager& mManager) : manager(mManager) {}

void Statusbar::init(Status status, bool animated) {

	SDL_Rect statusbarSpace = { 0, 0, 1200, status.height };

	statusbar = &manager.addEntity();

	statusbar->addComponent<PositionComponent>(statusbarSpace, 1);
	statusbar->addComponent<SpriteComponent>(status.texture, true);
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
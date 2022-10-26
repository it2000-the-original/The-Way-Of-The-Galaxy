#include "Statusbar.h"

void Widget::destroy() { active = false; }

bool Widget::isActive() { return active; }

int Widget::getWidth() {

	std::string widgetString = prefix + model;
	return widgetString.size() * statusbar->fontSize / 2;
}

int Widget::getIconWidth() {

	return iconSize + iconSpacing;
}

void Widget::addIcon(const char* iconPath, int mIconSize, int mIconSpacing, int iconMarginTop) {

	iconSize = mIconSize;
	iconSpacing = mIconSpacing;
	entity->getComponent<TextComponent>().addIcon(iconPath, iconSize, iconSpacing, iconMarginTop, false);
	statusbar->reloadPositions();
}

void Widget::setColor(int r, int g, int b, int a) {

	entity->getComponent<TextComponent>().setColor(r, g, b, a);
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

Widget::~Widget() {}

//////////////////////////////// Statusbar methods

Statusbar::Statusbar(Manager& mManager) : manager(mManager) { statusbar = &manager.addEntity(); }

void Statusbar::init(int size, const char* statusImage, std::string mFontPath, int mFontSize, int mTopPosition, int mSpacing, bool animated) {

	statusbar->addComponent<PositionComponent>(0, 0, 1200, size, 1);
	statusbar->addComponent<SpriteComponent>(statusImage, animated, true);
	statusbar->addGroup(groupStatus);
	fontPath = mFontPath;
	fontSize = mFontSize;
	spacing = mSpacing;
	topPosition = mTopPosition;
}

void Statusbar::update() {

	for (auto* widget : widgets) {widget->update();}
}

void Statusbar::refresh() {

	widgets.erase(std::remove_if(std::begin(widgets), std::end(widgets),
		[](Widget* mWidget) {

			if (!mWidget->isActive()) {

				mWidget->entity->destroy();
				delete mWidget;
				return true;
			}

			return false;
		}), 
		
		std::end(widgets));
}

void Statusbar::reloadPositions() {

	int lastPosition = spacing;

	for (auto* widget : widgets) {

		lastPosition += widget->getIconWidth();
		widget->setPosition(lastPosition, topPosition);
		lastPosition += widget->getWidth() + spacing;
	}
}

void Statusbar::setAnimation(int f, int i, int s) {

	statusbar->getComponent<SpriteComponent>().addAnimation("statusAnimation", f, i, s);
	statusbar->getComponent<SpriteComponent>().playAnimation("statusAnimation");
}
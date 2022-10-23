#include "Statusbar.h"

void Widget::destroy() { active = false; }

bool Widget::isActive() { return active; }

int Widget::getWidth() {

	std::string widgetString = prefix + model;
	int iconWidth = iconSize + iconSpacing;
	return (widgetString.size() * statusbar->fontSize / 2) + iconWidth;
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

void Widget::setColor(int r, int g, int b) {

	entity->getComponent<TextComponent>().setColor(r, g, b);
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

void Statusbar::init(int size, const char* statusImage, std::string mFontPath, int mFontSize, int mTopPosition, int mSpacing) {

	statusbar->addComponent<PositionComponent>(0, 0, 1200, size, 1);
	statusbar->addComponent<SpriteComponent>(statusImage);
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

	int lastPosition = widgets[0]->getIconWidth();

	for (auto* widget : widgets) {

		lastPosition += spacing;
		widget->setPosition(lastPosition, topPosition);
		lastPosition += widget->getWidth();
	}
}
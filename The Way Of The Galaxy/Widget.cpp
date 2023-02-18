#include "Widget.h"
#include "Statusbar.h"

// A function that return a string from a
// int number that show all digits in relation to the model
std::string stringNumber(std::string model, int number);

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

///////////////////////////////////////////// Defining different types of widgets........

// Energy Widget

EnergyWidget::EnergyWidget(int* mEnergy) { energy = mEnergy; }

void EnergyWidget::update() {

	entity->getComponent<TextComponent>().setText(prefix + stringNumber(model, *energy));
}

// Missiles Widget

MissilesWidget::MissilesWidget(int* mMissiles) { missiles = mMissiles; }

void MissilesWidget::update() {

	entity->getComponent<TextComponent>().setText(prefix + stringNumber(model, *missiles));
}

// Weapon Widget

WeaponWidget::WeaponWidget(PlayerSpaceship* mPlayer) { player = mPlayer; }

void WeaponWidget::update() {

	entity->getComponent<TextComponent>().setText(prefix + player->getSelectedWeapon());
}

///////////////////////////// External functions..............

std::string stringNumber(std::string model, int number) {

	std::string stringNumber = std::to_string(number);
	while (stringNumber.size() < model.size())
	stringNumber = "0" + stringNumber;
	return stringNumber;
}
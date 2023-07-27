#include "Statusbar.h"
#include "TextureManager.h"

using namespace nlohmann;

Statusbar::Statusbar() {}

Statusbar::Statusbar(std::string settingsPath) {

	std::ifstream file(settingsPath);

	try {

		json data = json::parse(file);

		width = data["width"].get<int>();
		height = data["height"].get<int>();

		texture = TextureManager::LoadTexture(data["texture"].get<std::string>().c_str());

		spacing = data["spacing"].get<int>();
		marginTop = data["marginTop"].get<int>();

		font = TTF_OpenFont(
			data["font"].get<std::string>().c_str(),
			data["fontSize"].get<int>()
		);

		if (data["animated"].get<bool>()) {

			animated = true;
			frames = data["animation"]["frames"].get<int>();
			speed = data["animation"]["speed"].get<int>();
		}

		initializable = true;
	}

	catch (json::parse_error& ex) {

		// Found problems in the file
		std::cerr << "Statusbar Error: Inexistent file or invalid settings " << ex.what() << std::endl;
	}
}

void Statusbar::init() {

	if (initializable) {

		SDL_Rect space = { 0, 0, width, height };

		statusbar = &Engine::manager.addEntity();

		statusbar->addComponent<PositionComponent>(space, 1);
		statusbar->addComponent<SpriteComponent>(texture, true);
		statusbar->addGroup(groupStatus);

		if (animated) {

			statusbar->getComponent<SpriteComponent>().addAnimation("statusAnimation", frames, 0, speed);
			statusbar->getComponent<SpriteComponent>().playAnimation("statusAnimation");
		}

		statusheight = height;

		std::cout << "Stutusbar initialized succesfuly" << std::endl;

		active = true;
	}

	else {

		std::cout << "Statusbar not initialized" << std::endl;
	}
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

	}), std::end(widgets));
}

void Statusbar::reloadPositions() {

	// Move all widgets into the correct position

	if (active) {

		int lastPosition = spacing;

		for (auto* widget : widgets) {

			lastPosition += widget->getIconWidth();
			widget->setPosition(lastPosition, marginTop);
			lastPosition += widget->getWidth() + spacing;
		}
	}
}
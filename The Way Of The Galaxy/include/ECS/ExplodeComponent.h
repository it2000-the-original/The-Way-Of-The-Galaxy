#pragma once
#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "Random.h"

// A struct created to reduce the number of parameters to pass
// to the ExplodeComponent constructor

struct Explosion {

	SDL_Texture* texture;
	int frames = 10;
	int duration = 50;
	int width = 32;
	int height = 32;
};

class ExplodeComponent : public Component {

private:

	PositionComponent* position;

	SDL_Texture* texture;

	int width = 64;
	int height = 64;

	bool pieces = false;
	int piecesNumber = 10;
	const int piecesSize = 10;

	int explosionFrames = 20;
	int explosionDuration = 50;

	// Some path of various pieces to select randomically

	const char* piecesPaths[1] {
		"sprites//scraps//pieces.png"
	};


public:

	ExplodeComponent() {}

	ExplodeComponent(Explosion explosion) {

		texture = explosion.texture;
		explosionFrames = explosion.frames;
		explosionDuration = explosion.duration;

		width = explosion.width;
		height = explosion.height;
	}

	ExplodeComponent(Explosion explosion, int mPiecesNumber) {

		texture = explosion.texture;
		explosionFrames = explosion.frames;
		explosionDuration = explosion.duration;

		width = explosion.width;
		height = explosion.height;

		pieces = true;
		piecesNumber = mPiecesNumber;
	}

	void init() override {

		if (!entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}

		position = &entity->getComponent<PositionComponent>();
	}

	void explode() {

		// set the point where spawn the explosion
		int xposition = int(position->position.x + (position->width / 2) - (width / 2));
		int yposition = int(position->position.y + (position->height / 2) - (height / 2));

		SDL_Rect explosionSpace = { xposition, yposition, width, height };

		auto& explosion = entity->manager.addEntity();

		explosion.addComponent<PositionComponent>(explosionSpace, 2);
		explosion.addComponent<SpriteComponent>(texture, true);
		explosion.addComponent<ExplosionComponent>(explosionFrames * explosionDuration);

		explosion.getComponent<SpriteComponent>().addAnimation("explosion", explosionFrames, 0, explosionDuration);
		explosion.getComponent<SpriteComponent>().playAnimation("explosion");
		
		explosion.addGroup(groupExplosions);

		if (pieces) {

			for (int i = 0; i <= piecesNumber; i++) {

				// generate a random direction wher move the piece
				float radiants = float(randomInRange(0, 364)) * float(3.1415 / 180.0);
				int speed = randomInRange(1, 6);

				int xspeed = int(round(float(speed) * sin(radiants)));
				int yspeed = int(round(float(speed) * cos(radiants)));

				// set the point where spawn the piece
				int xposition = int(position->position.x + (position->width / 2) - (piecesSize / 2));
				int yposition = int(position->position.y + (position->height / 2) - (piecesSize / 2));

				SDL_Rect pieceSpace = { xposition, yposition, piecesSize, piecesSize };

				// creating the piece entity

				auto& piece = entity->manager.addEntity();

				piece.addComponent<PositionComponent>(pieceSpace, 1);
				piece.addComponent<SpriteComponent>(piecesPaths[0], false);
				piece.addComponent<AutodestroyComponent>();

				piece.getComponent<PositionComponent>().setSpeed(xspeed, yspeed);
				piece.getComponent<PositionComponent>().setRotationSpeed(randomInRange(-15, 15));

				piece.addGroup(groupPieces);
			}
		}
		
		entity->destroy(); // After the explosion the entity must be destroyed
	}
};

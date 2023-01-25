#pragma once
#include "Components.h"
#include "Random.h"

// A struct created to reduce the number of parameters to pass
// to the ExplodeComponent constructor

struct Explosion {

	const char* path;
	int frames = 10;
	int frameDuration = 50;
};

class ExplodeComponent : public Component {

private:

	PositionComponent* position;

	int width = 64;
	int height = 64;

	bool pieces = false;
	int piecesNumber = 10;
	const int piecesSize = 10;

	const char* explosionPath;
	int explosionFrames = 20;
	int explosionFrameDuration = 50;

	// Some path of various pieces to select randomically

	const char* piecesPaths[1] {
		"sprites//other//pieces.png"
	};


public:

	ExplodeComponent() {}

	ExplodeComponent(Explosion explosion) {

		explosionPath = explosion.path;
		explosionFrames = explosion.frames;
		explosionFrameDuration = explosion.frameDuration;
	}

	ExplodeComponent(Explosion explosion, int mWidth, int mHeight) {

		explosionPath = explosion.path;
		explosionFrames = explosion.frames;
		explosionFrameDuration = explosion.frameDuration;

		width = mWidth;
		height = mHeight;
	}

	ExplodeComponent(Explosion explosion, int mWidth, int mHeight, int mPiecesNumber) {

		explosionPath = explosion.path;
		explosionFrames = explosion.frames;
		explosionFrameDuration = explosion.frameDuration;

		width = mWidth;
		height = mHeight;

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
		int xposition = position->position.x + (position->width / 2) - (width / 2);
		int yposition = position->position.y + (position->height / 2) - (height / 2);

		SDL_Rect explosionSpace = { xposition, yposition, width, height };

		auto& explosion = entity->manager.addEntity();

		explosion.addComponent<PositionComponent>(explosionSpace, 2);
		explosion.addComponent<SpriteComponent>(explosionPath, true);
		explosion.addComponent<ExplosionComponent>(explosionFrames * explosionFrameDuration);

		explosion.getComponent<SpriteComponent>().addAnimation("explosion", explosionFrames, 0, explosionFrameDuration);
		explosion.getComponent<SpriteComponent>().playAnimation("explosion");
		
		explosion.addGroup(groupExplosions);

		if (pieces) {

			for (int i = 0; i <= piecesNumber; i++) {

				// generate a random direction wher move the piece
				float radiants = float(randomInRange(0, 364)) * (3.1415 / 180.0);
				int speed = randomInRange(1, 6);

				int xspeed = int(round(float(speed) * sin(radiants)));
				int yspeed = int(round(float(speed) * cos(radiants)));

				// set the point where spawn the piece
				int xposition = position->position.x + (position->width / 2) - (piecesSize / 2);
				int yposition = position->position.y + (position->height / 2) - (piecesSize / 2);

				SDL_Rect pieceSpace = { xposition, yposition, piecesSize, piecesSize };

				// creating the piece entity

				auto& piece = entity->manager.addEntity();

				piece.addComponent<PositionComponent>(pieceSpace, 1, 0, false);
				piece.addComponent<SpriteComponent>(piecesPaths[0], false);
				piece.addComponent<BulletComponent>();

				piece.getComponent<PositionComponent>().setSpeed(xspeed, yspeed);
				piece.getComponent<PositionComponent>().setRotationSpeed(randomInRange(-15, 15));

				piece.addGroup(groupPieces);
			}
		}
		
		entity->destroy(); // After the explosion the entity must be destroyed
	}
};

#pragma once
#include "Components.h"
#include "Random.h"

class ExplodeComponent : public Component {

private:

	int width;
	int height;
	bool pieces;
	int piecesNumber;
	int explosionFrames;
	std::string explosionPath;
	int explosionFrameDuration;
	PositionComponent* position;

	const char* piecesPaths[1] {
		"sprites//other//pieces.png"
	};

public:

	ExplodeComponent(std::string mExplosionPath, int mWidth, int mHeight, int mExplosionFrames, int mExplosionFrameDuration) {

		explosionPath = mExplosionPath;
		width = mWidth;
		height = mHeight;
		explosionFrames = mExplosionFrames;
		explosionFrameDuration = mExplosionFrameDuration;
		pieces = false;
	}

	ExplodeComponent(std::string mExplosionPath, int mWidth, int mHeight, int mExplosionFrames, int mExplosionFrameDuration ,bool mPieces, int mPiecesNumber) {

		explosionPath = mExplosionPath;
		width = mWidth;
		height = mHeight;
		explosionFrames = mExplosionFrames;
		explosionFrameDuration = mExplosionFrameDuration;
		pieces = mPieces;
		piecesNumber = mPiecesNumber;
	}

	void init() override {

		position = &entity->getComponent<PositionComponent>();
	}

	void explode() {

		auto& explosion = entity->manager.addEntity();
		explosion.addComponent<PositionComponent>(position->position.x + position->width / 2 - width, position->position.y + position->height / 2 - height, width, height, 2);
		explosion.addComponent<SpriteComponent>(explosionPath.c_str(), true);
		explosion.getComponent<SpriteComponent>().addAnimation("explosion", explosionFrames, 0, explosionFrameDuration);
		explosion.getComponent<SpriteComponent>().playAnimation("explosion");
		explosion.addComponent<ExplosionComponent>(explosionFrames * explosionFrameDuration);
		explosion.addGroup(groupExplosions);

		if (pieces) {

			for (int i = 0; i <= piecesNumber; i++) {

				auto& piece = entity->manager.addEntity();
				piece.addComponent<PositionComponent>(position->position.x + position->width / 2 - 10, position->position.y + position->height / 2 - 10, 10, 10, 1, 0, false, randomInRange(-15, 15));
				piece.getComponent<PositionComponent>().setSpeed(randomNoZero(-3, 3), randomNoZero(-3, 3));
				piece.addComponent<SpriteComponent>(piecesPaths[0], false);
				piece.addComponent<BulletComponent>();
				piece.addGroup(groupPieces);
			}
		}
		
		entity->destroy();
	}
};

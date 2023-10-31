#include "ECS/Components.h"
#include "Explosion.h"
#include "Engine.h"
#include "Random.h"

ExplodeComponent::ExplodeComponent() {}

ExplodeComponent::ExplodeComponent(Explosion explosion) {

	texture = explosion.texture;
	explosionFrames = explosion.frames;
	explosionDuration = explosion.duration;

	width = explosion.width;
	height = explosion.height;
}

ExplodeComponent::ExplodeComponent(Explosion explosion, int mPiecesNumber) {

	texture = explosion.texture;
	explosionFrames = explosion.frames;
	explosionDuration = explosion.duration;

	width = explosion.width;
	height = explosion.height;

	pieces = true;
	piecesNumber = mPiecesNumber;
}

void ExplodeComponent::init() {

	if (!entity->hasComponent<PositionComponent>()) {

		entity->addComponent<PositionComponent>();
	}

	position = &entity->getComponent<PositionComponent>();
}

void ExplodeComponent::explode() {

	// set the point where spawn the explosion
	int xposition = int(position->position.x + (position->width / 2) - (width / 2));
	int yposition = int(position->position.y + (position->height / 2) - (height / 2));

	Rectangle explosionSpace = { double(xposition), double(yposition), double(width), double(height) };

	auto& explosion = entity->manager.addEntity();

	explosion.addComponent<PositionComponent>(explosionSpace, 2);
	explosion.addComponent<SpriteComponent>(texture);
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

			Rectangle pieceSpace = { double(xposition), double(yposition), double(piecesSize), double(piecesSize) };

			// creating the piece entity

			auto& piece = entity->manager.addEntity();

			piece.addComponent<PositionComponent>(pieceSpace, 1);
			piece.addComponent<SpriteComponent>(piecesPaths[0]);
			piece.addComponent<AutodestroyComponent>();

			piece.getComponent<PositionComponent>().setSpeed(xspeed, yspeed);
			piece.getComponent<PositionComponent>().setRotationSpeed(randomInRange(-15, 15));

			piece.addGroup(groupPieces);
		}
	}

	entity->destroy(); // After the explosion the entity must be destroyed
}

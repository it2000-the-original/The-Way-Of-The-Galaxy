#pragma once
#include "ECS.h"

class PositionComponent;
struct Explosion;

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

	ExplodeComponent();
	ExplodeComponent(Explosion explosion);
	ExplodeComponent(Explosion explosion, int mPiecesNumber);

	void init() override;

	void explode();
};

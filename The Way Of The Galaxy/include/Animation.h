#pragma once

struct Animation {

	int frames;
	int index;
	int speed;

	Animation() {}
	Animation(int f, int i, int s) {

		frames = f;
		index = i;
		speed = s;
	}
};

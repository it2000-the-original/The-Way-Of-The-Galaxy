#pragma once
#include <random>

// Generate a random number in the indicated range

inline int randomInRange(int min, int max) {

	return rand() % (max - min + 1) + min;
}

// Generate a random number in the indicated range excluding 0

inline int randomNoZero(int min, int max) {

	int randvar;
	do randvar = rand() % (max - min + 1) + min;
	while (randvar == 0);
	return randvar;
}

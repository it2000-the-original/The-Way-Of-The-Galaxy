#pragma once
#include <random>

inline int randomInRange(int min, int max) {

	return rand() % (max - min + 1) + min;
}

inline int randomNoZero(int min, int max) {

	int randvar;
	do randvar = rand() % (max - min + 1) + min;
	while (randvar == 0);
	return randvar;
}

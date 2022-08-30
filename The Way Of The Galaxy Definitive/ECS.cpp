#include "ECS.h"

// I have created this origin file because i cant't declare functions that call
// a metod of a class that is not defined, because it generate an error.
// That is because i have adapted this non-clean solution.

void Entity::addGroup(int mGroup) {

	GroupBitSet[mGroup] = true;
	manager.addToGroup(this, mGroup);
}
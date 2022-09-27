#pragma once
#include "Components.h"

class MissileComponent : public Component {

public:

	int damage;

	MissileComponent() {
		
		damage = 5;
	}

	MissileComponent(int mDamage) {
		
		damage = mDamage;
	}
};

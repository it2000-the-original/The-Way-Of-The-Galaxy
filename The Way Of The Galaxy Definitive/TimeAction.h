#pragma once
#include <SDL2/SDL.h>

class TimeAction {

private:

	Uint32 initialTime;
	Uint32 duration;
	bool active;

public:

	TimeAction();
	TimeAction(Uint32 mDuration);
	void init();
	bool check();
	bool isActive();
	void setDuration(Uint32 mDuration);
};

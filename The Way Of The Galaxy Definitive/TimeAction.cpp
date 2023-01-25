#include "TimeAction.h"

TimeAction::TimeAction() { duration = 1000; }

TimeAction::TimeAction(Uint32 mDuration) { 
	
	duration = mDuration;
	active = true;
}

void TimeAction::init() {

	// start counting the time
	initialTime = SDL_GetTicks();
	active = true;
}

bool TimeAction::check() {

	// Check if the time from init is past

	if (active and SDL_GetTicks() - initialTime > duration) {

		active = false;
		return true;
	}

	return false;
}

bool TimeAction::isActive() {

	// Check if the time started to counting

	return active;
}

void TimeAction::setDuration(Uint32 mDuration) { 
	
	duration = mDuration;
}
#include "TimeAction.h"

TimeAction::TimeAction() { duration = 1000; }

TimeAction::TimeAction(Uint32 mDuration) { duration = mDuration; active = true; }

void TimeAction::init() {

	initialTime = SDL_GetTicks();
	active = true;
}

bool TimeAction::check() {

	if (active and SDL_GetTicks() - initialTime > duration) {

		active = false;
		return true;
	}

	return false;
}

bool TimeAction::isActive() {

	return active;
}

void TimeAction::setDuration(Uint32 mDuration) { duration = mDuration; }
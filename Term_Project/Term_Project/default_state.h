#pragma once
#include "state.h"

class Default_state : public  State {
	// 사용할 객체들 선언
	int notes;
public:
	virtual void enter();
	virtual void pause();
	virtual void resume();
	virtual void exit();
	virtual void handle_events(Event evnt);
	virtual void update();
	virtual void draw();
};
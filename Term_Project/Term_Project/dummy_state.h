#pragma once
#include "state.h"

class Dummy_state : public  State {
	// 사용할 객체들 선언
	int notes;
public:
	virtual void enter();
	virtual void update();
	virtual void draw();
};
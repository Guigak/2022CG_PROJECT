#pragma once
#include "state.h"

class Dummy_state : public  State {
	// ����� ��ü�� ����
	int notes;
public:
	virtual void enter();
	virtual void update();
	virtual void draw();
};
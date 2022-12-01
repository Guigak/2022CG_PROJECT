#pragma once
#include "event.h"
#include <iostream>

using namespace std;

class State {

public:
	virtual void enter() { cout << "enter\n"; }
	virtual void pause() { cout << "pause\n"; }
	virtual void resume() { cout << "resume\n"; }
	virtual void exit() { cout << "exit\n"; }
	virtual void handle_events(Event evnt) { cout << "handle_events\n"; }
	virtual void update() { cout << "update\n"; }
	virtual void draw() { cout << "draw\n"; }
};
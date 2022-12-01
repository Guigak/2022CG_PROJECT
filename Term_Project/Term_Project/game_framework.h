#pragma once
#include "event.h"
#include "stack.h"
#include "state.h"
#include <ctime>

class Game_Framework {
	bool running = false;
	Stack<State*> stack;
	double frame_time = 0.0;
	clock_t current_time = clock();

public:
	double get_frame_time();

	void init(State* start_state);

	void change_state(State* state);

	void push_state(State* state);

	void pop_state();

	void quit();

	void handle_events(Event evnt);

	void draw();

	void run();
};

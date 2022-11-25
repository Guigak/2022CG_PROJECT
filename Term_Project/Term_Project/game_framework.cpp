#include <iostream>
#include <ctime>
#include "event_class.cpp"

#define MAX_SIZE (10000)
using namespace std;

template <typename T>

class Stack {
private:
	T data[MAX_SIZE];
	int _top;

public:
	Stack() {
		_top = -1;
	}
	~Stack() {

	}
	void push(T inputdata) {
		data[_top + 1] = inputdata;
		_top += 1;
	}
	T pop() {
		T res = data[_top];
		_top -= 1;
		return res;
	}
	int size() {
		return _top + 1;
	}
	bool empty() {
		return size() == 0 ? true : false;
	}
	T top() {
		T res = data[_top];
		return res;
	}
};

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

/*
	Game_Framework main;
	main.run(play_state);
*/

class Game_Framework {
	bool running = false;
	Stack<State> stack;
	double frame_time = 0.0;
	clock_t current_time = clock();

public:
	double get_frame_time() { return frame_time; }

	void init(State start_state) {
		running = true;
		stack.push(start_state);
		start_state.enter();
		current_time = clock();
	}

	void change_state(State state) {
		if (stack.size() > 0) {
			stack.top().exit();
			stack.pop();
			stack.push(state);
			state.enter();
		}
	}

	void push_state(State state) {
		if (stack.size() > 0) {
			stack.top().pause();
			stack.push(state);
			state.enter();
		}
	}

	void pop_state() {
		if (stack.size() > 0) {
			stack.top().exit();
			stack.pop();

		}
		if (stack.size() > 0) {
			stack.top().resume();
		}
	}

	void quit() {
		running = false;
	}

	void handle_events(Event evnt) {
		stack.top().handle_events(evnt);
	}

	void draw() {
		stack.top().draw();
	}

	void run() {
		if (running) {
			stack.top().update();
			frame_time = (double)(clock() - current_time);
			current_time += frame_time;
		}
		else {
			while (stack.size() > 0) {
				stack.top().exit();
				stack.pop();
			}
		}
	}
};



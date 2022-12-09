#include "game_framework.h"

Game_Framework game_framework;

Game_Framework& Get_Game_Framework() { return game_framework; }

double Game_Framework::get_frame_time() { return frame_time; }

void Game_Framework::init(State* start_state, GLuint program, GLuint* a, GLuint* b) {
	running = true;

	shader_program = program;
	vao = a;
	vbo = b;

	stack.push(start_state);
	start_state->enter(shader_program, vao, vbo);
	current_time = clock();
}

void Game_Framework::change_state(State* state) {
	if (stack.size() > 0) {
		stack.top()->exit();
		stack.pop();
		stack.push(state);
		state->enter(shader_program, vao, vbo);
	}
}

void Game_Framework::change_state(State* state, GLint song) {

	if (stack.size() > 0) {
		stack.top()->exit();
		stack.pop();
		stack.push(state);
		state->enter(shader_program, vao, vbo, song);
	}
}

void Game_Framework::push_state(State* state) {
	if (stack.size() > 0) {
		stack.top()->pause();
		stack.push(state);
		state->enter(shader_program, vao, vbo);
	}
}

void Game_Framework::pop_state() {
	if (stack.size() > 0) {
		stack.top()->exit();
		stack.pop();

	}
	if (stack.size() > 0) {
		stack.top()->resume();
	}
}

void Game_Framework::quit() {
	running = false;
}

void Game_Framework::handle_events(Event evnt) {
	stack.top()->handle_events(evnt);
}

void Game_Framework::draw() {
	stack.top()->draw();
}

void Game_Framework::run() {
	if (running) {
		stack.top()->update();
		frame_time = (double)(clock() - current_time) / CLOCKS_PER_SEC;
		current_time += frame_time * CLOCKS_PER_SEC;
	}
	else {
		while (stack.size() > 0) {
			stack.top()->exit();
			stack.pop();
		}
		std::exit(0);
	}
}



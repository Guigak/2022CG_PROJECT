#include "dummy_state.h"

void Dummy_state::enter() {
	cout << "dummy_enter\n";
}

void Dummy_state::handle_events(Event evnt) {
	cout << "dummy_handle_events\n";
}

void Dummy_state::update() {
	cout << "dummy_update\n";
}

void Dummy_state::draw() {
	cout << "dummy_draw\n";
}

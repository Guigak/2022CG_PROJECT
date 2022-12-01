#pragma once

enum event_type { MOUSE, KEYBOARD, KEYBOARD_UP, SPECIAL, SPECIAL_UP, MOTION };

struct Event {
	// ���� �̺�Ʈ���� 
	event_type type;
	// Ű���� ����
	unsigned char key;
	// Ű���� Ư��Ű(����Ʈ ��) or ���콺 ��ư(�� or ��)
	int special_key;
	// ���콺 ��ǥ
	int x, y;
	// ���콺 ����(�� �ٿ�)
	int state;

	explicit Event(event_type t_type, unsigned char t_key, int t_special_key, int t_x, int t_y, int t_state) {
		type = t_type;
		key = t_key;
		special_key = t_special_key;
		x = t_x;
		y = t_y;
		state = t_state;
	}
};
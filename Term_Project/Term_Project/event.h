#pragma once

enum event_type { MOUSE, KEYBOARD, KEYBOARD_UP, SPECIAL, SPECIAL_UP, MOTION };

struct Event {
	// 무슨 이벤트인지 
	event_type type;
	// 키보드 문자
	unsigned char key;
	// 키보드 특수키(쉬프트 등) or 마우스 버튼(왼 or 오)
	int special_key;
	// 마우스 좌표
	int x, y;
	// 마우스 상태(업 다운)
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
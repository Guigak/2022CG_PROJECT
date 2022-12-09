#pragma once

#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <fstream>
#include <string>

#include "game_framework.h"

#include "state.h"

#include "vertices.h"

#define TUM_RADIAN 10
#define OPENINGTIME 1000
#define MAX_OPTIONS 2 // 옵션의 개수 note speed, volumn


class Option_state : public  State {
private:
	// default
	GLuint shader_program;
	GLuint* vao;
	GLuint* vbo;

	// camera
	GLfloat camera_x;
	GLfloat camera_y;
	GLfloat camera_z;

	// ight
	GLfloat brightness;

	// system
	GLint camera_radian;	// * 3.0f

	GLint max_selnum;
	GLint selected_num;	
	GLint option_sel_num = 0; // note speed, volume
	GLint Turning;
	
	GLint state; // 0 state, 1 run, 2 end

	State* next_state;
	// data
	GLfloat note_speed, volume;


public:
	Option_state() {
		ifstream option_data("option_data.txt");
		if (option_data.fail()) {
			ofstream init_data("option_data.txt");
			init_data << 1.0 << ' ' << 5;
			note_speed = 1.0;
			volume = 5;
			return;
		}
		option_data >> note_speed >> volume;
	}
	// default
	virtual void enter(GLuint, GLuint*, GLuint*);	// shader program, vao, vbo
	virtual void pause();
	virtual void resume();
	virtual void exit();
	virtual void handle_events(Event evnt);
	virtual void update();
	virtual void draw();
	// useful
	virtual void GenBuffer();
	virtual void InitBuffer();

	// system
};

Option_state& Get_Option_state();

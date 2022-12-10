#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "game_framework.h"

#include "state.h"
#include "second_state.h"

#include "vertices.h"

#define TUM_RADIAN 10
#define OPENINGTIME 1000

class Title_state : public  State {
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
	GLint selected_num;	// 0 - start, 1 - option

	GLint Turning;

	GLint state; // 0 start, 1 run, 2 end
	State* next_state;

	GLint mode;

	// command
	GLint Made;

	// fmod
	FMOD_SYSTEM* soundSystem; //--- FMOD system 변수 선언
	FMOD_SOUND* soundFile; //--- 사운드 객체 선언
	FMOD_CHANNEL* channel; //--- 채널 선언


public:
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

Title_state& Get_Title_state();

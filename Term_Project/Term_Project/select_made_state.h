#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <fmod.h>

#include "game_framework.h"

#include "state.h"

#include "vertices.h"

#define TUM_RADIAN 10
#define OPENINGTIME 1000

class Select_Made_state : public  State {
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
	GLint song_num;

	GLint mode_num;	// 0 - play, 1 - made, 2 - test

	GLint Turning;

	GLint state; 
	State* next_state;


	// FMOD
	FMOD_SYSTEM* soundsystem;

	FMOD_SOUND* soul;
	FMOD_SOUND* animal;
	FMOD_SOUND* sea;

	FMOD_CHANNEL* bgc;

	GLboolean Soundplaying;

public:
	// default
	virtual void enter(GLuint, GLuint*, GLuint*, GLint);	// shader program, vao, vbo
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

Select_Made_state& Get_Select_Made_state();

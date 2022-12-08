#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "state.h"

#include "vertices.h"

#define TUM_RADIAN 10

class Second_state : public  State {
private:
	// default
	GLuint shader_program;
	GLuint vao[3];
	GLuint vbo[3];

	// camera
	GLfloat camera_x;
	GLfloat camera_y;
	GLfloat camera_z;

	// system
	GLint camera_radian;	// * 3.0f

	GLint max_selnum;
	GLint selected_num;	// 0 - start, 1 - option

	GLint Turning;

public:
	// default
	virtual void enter(GLuint);	// shader program, vao, vbo
	virtual void pause();
	virtual void resume();
	virtual void exit();
	virtual void handle_events(Event evnt);
	virtual void update();
	virtual void draw();
	// useful
	virtual void GenBuffer();
	virtual void InitBuffer();
};

Second_state& Get_Second_state();

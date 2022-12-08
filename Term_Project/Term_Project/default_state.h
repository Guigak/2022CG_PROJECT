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

class Default_state : public  State {
private:
	// default
	GLuint shader_program;
	GLuint vao[3];
	GLuint vbo[3];

	// camera
	GLfloat camera_x;
	GLfloat camera_y;
	GLfloat camera_z;
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
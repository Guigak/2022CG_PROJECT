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
#define MAX_NOTE 1000

class NOTEINFO {
public:
	glm::mat4 Trans_time = glm::mat4(1.0f);
	glm::mat4 Trans_line = glm::mat4(1.0f);
	glm::mat4 Rotate_line = glm::mat4(1.0f);
	GLint playline = 0;
};

class Test_state : public  State {
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

	GLint state;

	clock_t start_time;
	GLfloat play_time;
	glm::mat4 Trans_playtime;

	GLboolean Iswrited;

	State* next_state;

	// FMOD
	FMOD_SYSTEM* soundsystem;

	FMOD_SOUND* soul;
	FMOD_SOUND* insta;
	FMOD_SOUND* kiss;

	FMOD_CHANNEL* bgc;

	GLboolean Soundplaying;

	GLint selected_song;

	// notes
	NOTEINFO noteinfos[MAX_NOTE];
	GLint note_num;
	GLint max_notenum;

	GLboolean IspressedA;
	GLboolean IspressedS;
	GLboolean IspressedD;
	GLboolean IspressedF;

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
	void read_file();
};

Test_state& Get_Test_state();

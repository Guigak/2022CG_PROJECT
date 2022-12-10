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
#define TURN_SPEED 250.0

class NOTEINFOS {
public:
	glm::mat4 Trans_time = glm::mat4(1.0f);
	glm::mat4 Trans_line = glm::mat4(1.0f);
	glm::mat4 Rotate_line = glm::mat4(1.0f);
	GLint noteline = 0;
	GLint playline = 0;
	GLboolean Trigger = GL_FALSE;
	GLboolean Processed = GL_FALSE;
};

class Play_state : public  State {
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
	GLfloat camera_radian;	// * 3.0f

	GLint max_selnum;
	GLint selected_num;	// 0 - start, 1 - option

	GLint Turning;

	GLint state;

	clock_t start_time;
	GLfloat play_time;
	clock_t end_time;
	glm::mat4 Trans_playtime;

	GLfloat note_speed;
	GLfloat volume;
	glm::mat4 Scale_speed;

	GLint combo_num;
	GLint good_num;
	GLint miss_num;

	GLboolean Warning;

	State* next_state;

	// FMOD
	FMOD_SYSTEM* soundsystem;

	FMOD_SOUND* soul;
	FMOD_SOUND* animal;
	FMOD_SOUND* sea;

	FMOD_CHANNEL* bgc;

	GLboolean Soundplaying;

	GLint selected_song;

	// notes
	NOTEINFOS noteinfos[MAX_NOTE];
	GLint note_num;
	GLint max_notenum;

	GLint draw_notenum;
	GLint trigger_notenum;

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
	void process_note(GLint);
};

Play_state& Get_Play_state();

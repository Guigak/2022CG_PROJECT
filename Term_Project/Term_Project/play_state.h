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

#define NOTE_X_FIRST -0.75
#define NOTE_TUM 0.5

#define MAX_EFFECT 50

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

class EFFECTINFO {
public:
	GLfloat alpha_num = 0.0;
	GLfloat color[3] = { 0 };
	glm::mat4 Scale_time = glm::mat4(1.0f);
	glm::mat4 Trans_line = glm::mat4(1.0f);
	glm::mat4 Rotate_line = glm::mat4(1.0f);
	GLboolean Ison = GL_FALSE;

	void turn_on(GLint nline, GLint pline) {
		alpha_num = 0.5;

		if (pline == 0) {
			color[0] = 1.0;
			color[1] = color[2] = 0.0;
		}
		else {
			color[2] = 1.0;
			color[0] = color[1] = 0.0;
		}

		Scale_time = glm::mat4(1.0f);
		Trans_line = glm::mat4(1.0f);
		Rotate_line = glm::mat4(1.0f);

		Trans_line = glm::translate(Trans_line, glm::vec3(NOTE_X_FIRST + NOTE_TUM * (float)nline, 0.0, 0.0));
		Rotate_line = glm::rotate(Rotate_line, glm::radians(-30.0f * (float)pline), glm::vec3(0.0, 1.0, 0.0));
		Ison = GL_TRUE;
	}

	void update_(GLfloat ftime) {
		alpha_num -= ftime * 2.0;

		GLfloat scale_num = 1.5 - alpha_num;
		Scale_time = glm::mat4(1.0f);
		Scale_time = glm::scale(Scale_time, glm::vec3(scale_num, scale_num, scale_num));

		if (alpha_num < 0.0) {
			Ison = GL_FALSE;
		}
	}
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
	GLint mode;

	clock_t start_time;
	GLfloat play_time;
	clock_t end_time;
	glm::mat4 Trans_playtime;

	GLfloat note_speed;
	GLfloat volume;
	glm::mat4 Scale_speed;

	GLint combo_num;
	GLint max_combo;
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

	EFFECTINFO effectinfos[MAX_EFFECT];

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

	void add_effect(GLint, GLint);
	void update_effect();

	GLint Get_Good_Num() { return good_num; }
	GLint Get_Miss_Num() { return miss_num; }
	GLint Get_Max_Combo() { return max_combo; }

};

Play_state& Get_Play_state();

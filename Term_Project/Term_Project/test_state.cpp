#define _CRT_SECURE_NO_WARNINGS

#include "test_state.h"
#include "select_made_state.h"

#define NOTE_X_FIRST -0.75
#define NOTE_TUM 0.5

Test_state test_state;

Test_state& Get_Test_state() { return test_state; }

void Test_state::enter(GLuint program, GLuint* a, GLuint* b, GLint s) {
	shader_program = program;
	vao = a;
	vbo = b;

	camera_x = 0.0;
	camera_y = 1.0;
	camera_z = -3.0;

	brightness = 0.0;

	camera_radian = 0;

	max_selnum = 1;
	selected_num = 0;
	Turning = GL_FALSE;

	//fmod
	FMOD_System_Create(&soundsystem);
	FMOD_System_Init(soundsystem, 32, FMOD_INIT_NORMAL, NULL);

	FMOD_System_CreateSound(soundsystem, "Soulicious.mp3", FMOD_LOOP_OFF, 0, &soul);
	FMOD_System_CreateSound(soundsystem, "Animal_athletic_meeting.mp3", FMOD_LOOP_OFF, 0, &animal);
	FMOD_System_CreateSound(soundsystem, "The_sea.mp3", FMOD_LOOP_OFF, 0, &sea);

	Soundplaying = GL_FALSE;

	selected_song = s;

	for (int i = 0; i < MAX_NOTE; ++i) {
		noteinfos[i].Trans_time = glm::mat4(1.0f);
		noteinfos[i].Trans_line = glm::mat4(1.0f);
		noteinfos[i].Rotate_line = glm::mat4(1.0f);
		noteinfos[i].playline = 0;
	}

	note_num = 0;
	max_notenum = 0;

	IspressedA = GL_FALSE;
	IspressedS = GL_FALSE;
	IspressedD = GL_FALSE;
	IspressedF = GL_FALSE;

	// file
	read_file();

	//GenBuffer();
	InitBuffer();
	state = 0;
	next_state = nullptr;
}

void Test_state::pause() {

}

void Test_state::resume() {

}

void Test_state::exit() {
	FMOD_Channel_Stop(bgc);
	FMOD_Sound_Release(soul);
	FMOD_Sound_Release(animal);
	FMOD_Sound_Release(sea);
	FMOD_System_Close(soundsystem);
	FMOD_System_Release(soundsystem);
}

void Test_state::handle_events(Event evnt) {
	switch (evnt.type) {
	case KEYBOARD:
	{
		switch (evnt.key) {
		case 'a':
			if (!IspressedA) {

				IspressedA = GL_TRUE;
			}
			break;
		case 's':
			if (!IspressedS) {

				IspressedS = GL_TRUE;
			}
			break;
		case 'd':
			if (!IspressedD) {

				IspressedD = GL_TRUE;
			}
			break;
		case 'f':
			if (!IspressedF) {

				IspressedF = GL_TRUE;
			}
			break;
		case 13:
			if (!Iswrited) {
				Iswrited = GL_TRUE;
			}
			break;
		case 27:
			if (!Turning) {
				state = 2;
				next_state = &Get_Select_Made_state();
			}
			break;
		default:
			break;
		}
	}
	break;
	case KEYBOARD_UP:
	{
		switch (evnt.key) {
		case 'a':
			IspressedA = GL_FALSE;
			break;
		case 's':
			IspressedS = GL_FALSE;
			break;
		case 'd':
			IspressedD = GL_FALSE;
			break;
		case 'f':
			IspressedF = GL_FALSE;
			break;
		default:
			break;
		}
	}
	break;
	case SPECIAL:
	{
		switch (evnt.special_key) {
		case GLUT_KEY_LEFT:
			if (selected_num == 1) {
				selected_num = 0;
				Turning = -1;
			}
			break;
		case GLUT_KEY_RIGHT:
			if (selected_num == 0) {
				selected_num = 1;
				Turning = 1;
			}
			break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}
}

void Test_state::update() {
	switch (state)
	{
	case 0:
		brightness += Get_Game_Framework().get_frame_time() / 2;
		if (brightness >= 1.0) {
			brightness = 1.0;
			state = 1;

			//
			switch (selected_song) {
			case 0:
				FMOD_System_PlaySound(soundsystem, soul, NULL, 0, &bgc);
				break;
			case 1:
				FMOD_System_PlaySound(soundsystem, animal, NULL, 0, &bgc);
				break;
			case 2:
				FMOD_System_PlaySound(soundsystem, sea, NULL, 0, &bgc);
				break;
			default:
				break;
			}

			start_time = clock();
			FMOD_Channel_SetVolume(bgc, 0.25);
			Trans_playtime = glm::mat4(1.0f);
		}
		break;
	case 1:
		// turn //
		if (Turning == -1) {
			camera_radian -= 1;

			if (camera_radian % TUM_RADIAN == 0) {
				Turning = 0;
			}
		}
		else if (Turning == 1) {
			camera_radian += 1;

			if (camera_radian % TUM_RADIAN == 0) {
				Turning = 0;
			}
		}

		// time
		Trans_playtime = glm::mat4(1.0f);
		play_time = (float)(clock() - start_time) / 100.0;
		Trans_playtime = glm::translate(Trans_playtime, glm::vec3(0.0, 0.0, play_time));
		break;
	case 2:
		brightness -= Get_Game_Framework().get_frame_time() / 2;
		if (brightness <= 0.0) {
			for (int i = 0; i < MAX_NOTE; ++i) {
				noteinfos[i].Trans_time = glm::mat4(1.0f);
				noteinfos[i].Trans_line = glm::mat4(1.0f);
				noteinfos[i].Rotate_line = glm::mat4(1.0f);
				noteinfos[i].playline = 0;
			}

			if (next_state != nullptr) {
				Get_Game_Framework().change_state(next_state, 2);
			}
			else {
				exit();
			}
		}
		break;
	default:
		cout << "state?? start, run, end ?????? ???? ?????????? ???? ???? ????" << endl;
		break;
	}
}

void Test_state::draw() {
	glViewport(0, 0, glutGet(0x0066), glutGet(0x0067));
	// init //
	InitBuffer();

	//--- ?????? ?????? ????
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 TR = glm::mat4(1.0f);

	// program //
	glUseProgram(shader_program);

	// camera //

	glm::vec3 cameraPos = glm::vec3(camera_x, camera_y, camera_z); //--- ?????? ????
	glm::vec3 cameraDirection = glm::vec3(camera_x, camera_y, camera_z - 1.0); //--- ?????? ???????? ????
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ?????? ???? ????
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 TR_O = glm::mat4(1.0f);
	glm::mat4 TR_P = glm::mat4(1.0f);
	TR_O = glm::translate(TR_O, glm::vec3(-camera_x, -camera_y, -camera_z));
	TR_P = glm::translate(TR_P, glm::vec3(camera_x, camera_y, camera_z));

	glm::mat4 Rotate_Camera = glm::mat4(1.0f);
	Rotate_Camera = glm::rotate(Rotate_Camera, glm::radians((float)camera_radian * 3.0f), glm::vec3(0.0, 1.0, 0.0));

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	//view = TR_P * Rotate_Camera * TR_O * view;
	view = view * Rotate_Camera;

	unsigned int viewLocation = glGetUniformLocation(shader_program, "view"); //--- ???? ???? ????
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// projection //

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, MAX_PROJECT_Z);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0)); //--- ?????? ???? ???? ??????
	unsigned int projectionLocation = glGetUniformLocation(shader_program, "proj"); //--- ???? ???? ?? ????
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	// light //

	unsigned int lightPosLocation = glGetUniformLocation(shader_program, "lightPos"); //--- lightPos ?? ????: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 2.0, 0.0);

	int lightColorLocation = glGetUniformLocation(shader_program, "lightColor"); //--- lightColor ?? ????: (1.0, 1.0, 1.0) ????
	glUniform3f(lightColorLocation, brightness, brightness, brightness);

	unsigned int viewPosLocation = glGetUniformLocation(shader_program, "viewPos"); //--- viewPos ?? ????: ?????? ????
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	float ambientLightLocation = glGetUniformLocation(shader_program, "ambientLight");	// ambientLight~
	glUniform1f(ambientLightLocation, 0.5);

	// default //

	glEnable(GL_DEPTH_TEST);

	unsigned int modelLocation = glGetUniformLocation(shader_program, "model");

	int objColorLocation = glGetUniformLocation(shader_program, "objectColor"); //--- object Color?? ????

	int vectorLocation = glGetUniformLocation(shader_program, "tNormal"); // vector

	unsigned int IsText = glGetUniformLocation(shader_program, "IsText");

	// play line //
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	TR = glm::mat4(1.0f);
	glm::mat4 TR_r1 = glm::mat4(1.0f);
	glm::mat4 TR_t = glm::mat4(1.0f);
	glm::mat4 TR_r2 = glm::mat4(1.0f);

	TR_r1 = glm::rotate(TR_r1, glm::radians(20.0f), glm::vec3(1.0, 0.0, 0.0));
	TR_t = glm::translate(TR_t, glm::vec3(0.0, 0.0, camera_z - 3.0));
	TR_r2 = glm::rotate(TR_r2, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));

	TR = TR_t * TR_r1 * TR;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);


	for (int k = 0; k <= max_selnum; ++k) {
		// play line //
		glBindVertexArray(vao[0]);

		glUniform3f(objColorLocation, 0.0, 1.0, 0.0);
		glUniform1i(IsText, 0);

		for (int i = 0; i < 12; ++i) {
			GLfloat* tcube_vt = cube_vt[i % 6];
			glm::vec3 nVector = glm::mat3(glm::transpose(glm::inverse(TR))) * glm::vec3(tcube_vt[0], tcube_vt[1], tcube_vt[2]);
			glUniform3f(vectorLocation, nVector.x, nVector.y, nVector.z);

			for (int j = 2 * i; j < 2 * i + 2; ++j) {
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &line_l[j]);
			}
		}

		// trigger rect //
		glBindVertexArray(vao[1]);

		glUniform3f(objColorLocation, 1.0, 1.0, 0.0);

		for (int i = 0; i < 6; ++i) {
			GLfloat* tcube_vt = cube_vt[i % 6];
			glm::vec3 nVector = glm::mat3(glm::transpose(glm::inverse(TR))) * glm::vec3(tcube_vt[0], tcube_vt[1], tcube_vt[2]);
			glUniform3f(vectorLocation, nVector.x, nVector.y, nVector.z);

			for (int j = 2 * i; j < 2 * i + 2; ++j) {
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &line_l[j]);
			}
		}

		TR = TR_r2 * TR;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);
	}

	// note //
	glBindVertexArray(vao[2]);

	for (int k = 0; k < max_notenum; ++k) {
		if (noteinfos[k].playline == 0) {
			glUniform3f(objColorLocation, 1.0, 0.0, 0.0);
		}
		else {
			glUniform3f(objColorLocation, 0.0, 0.0, 1.0);
		}

		TR = glm::mat4(1.0f);
		TR = noteinfos[k].Rotate_line * TR_t * TR_r1 * noteinfos[k].Trans_line * Trans_playtime * noteinfos[k].Trans_time * TR;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);

		for (int i = 0; i < 6; ++i) {
			GLfloat* tcube_vt = cube_vt[i % 6];
			glm::vec3 nVector = glm::mat3(glm::transpose(glm::inverse(TR))) * glm::vec3(tcube_vt[0], tcube_vt[1], tcube_vt[2]);
			glUniform3f(vectorLocation, nVector.x, nVector.y, nVector.z);

			for (int j = 2 * i; j < 2 * i + 2; ++j) {
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &line_l[j]);
			}
		}
	}

	glutSwapBuffers();
}

void Test_state::GenBuffer() {
	glGenVertexArrays(3, vao);
	glGenBuffers(3, vbo);
}

void Test_state::InitBuffer() {
	// play line //
	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_v), line_v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// trigger line //
	glBindVertexArray(vao[1]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trigger_v), trigger_v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// note //
	glBindVertexArray(vao[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(note_v), note_v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void Test_state::read_file() {
	FILE* fp;

	switch (selected_song) {
	case 0:
		fp = fopen("soul.txt", "r");
		break;
	case 1:
		fp = fopen("animal.txt", "r");
		break;
	case 2:
		fp = fopen("sea.txt", "r");
		break;
	default:
		fp = fopen("error.txt", "r");
		break;
	}

	if (fp == NULL) {
		std::cout << "???? ???????? ????" << std::endl;
		return;
	}

	GLint now_notenum = 0;

	GLfloat note_time = 0.0;
	GLint note_line = 0;
	GLint play_line = 0;

	fscanf(fp, "%f", &note_time);

	while (note_time != 0.0) {
		fscanf(fp, "%d %d", &note_line, &play_line);

		noteinfos[now_notenum].Trans_time = glm::translate(noteinfos[now_notenum].Trans_time, glm::vec3(0.0, 0.0, -note_time));
		noteinfos[now_notenum].Trans_line = glm::translate(noteinfos[now_notenum].Trans_line, glm::vec3(NOTE_X_FIRST + NOTE_TUM * (float)note_line, 0.0, 0.0));
		noteinfos[now_notenum].Rotate_line = glm::rotate(noteinfos[now_notenum].Rotate_line, glm::radians(-30.0f * (float)play_line), glm::vec3(0.0, 1.0, 0.0));
		noteinfos[now_notenum].playline = play_line;

		max_notenum++;
		now_notenum++;

		fscanf(fp, "%f", &note_time);
	}

	std::cout << "???? ???????? ????" << std::endl;

	fclose(fp);
}


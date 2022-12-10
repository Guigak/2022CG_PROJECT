#define _CRT_SECURE_NO_WARNINGS

#include "Play_state.h"
#include "option_state.h"
#include "select_made_state.h"
#include "end_state.h"

#define NOTE_X_FIRST -0.75
#define NOTE_TUM 0.5
#define NOTE_Z_MIDDLE -0.125
#define TRIGGER_TUM 0.75
#define ANOTHER_Z 10.0
#define MAX_NOTE_Z -25.0f

Play_state play_state;

Play_state& Get_Play_state() { return play_state; }

void Play_state::enter(GLuint program, GLuint* a, GLuint* b, GLint s) {
	shader_program = program;
	vao = a;
	vbo = b;

	camera_x = 0.0;
	camera_y = 1.0;
	camera_z = -3.0;

	brightness = 0.0;

	camera_radian = 0.0;

	max_selnum = 1;
	selected_num = 0;
	Turning = GL_FALSE;

	start_time = 0;
	play_time = 0.0;
	end_time = 0;
	Trans_playtime = glm::mat4(1.0f);

	note_speed = (float)Get_Option_state().Get_Note_Speed() / 10.0;
	volume = (float)Get_Option_state().Get_Volume() / 100.0;
	Scale_speed = glm::mat4(1.0f);
	Scale_speed = glm::scale(Scale_speed, glm::vec3(1.0, 1.0, note_speed));

	combo_num = 0;
	good_num = 0;
	miss_num = 0;

	Warning = GL_FALSE;

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
		noteinfos[i].noteline = 0;
		noteinfos[i].playline = 0;
		noteinfos[i].Trigger = GL_FALSE;
		noteinfos[i].Processed = GL_FALSE;
	}

	note_num = 0;
	max_notenum = 0;

	draw_notenum = 0;
	trigger_notenum = 0;

	IspressedA = GL_FALSE;
	IspressedS = GL_FALSE;
	IspressedD = GL_FALSE;
	IspressedF = GL_FALSE;

	mode = -1;

	// file
	read_file();

	//GenBuffer();
	InitBuffer();
	state = 0;
	next_state = nullptr;
}

void Play_state::pause() {

}

void Play_state::resume() {

}

void Play_state::exit() {
	for (int i = 0; i < MAX_NOTE; ++i) {
		noteinfos[i].Trans_time = glm::mat4(1.0f);
		noteinfos[i].Trans_line = glm::mat4(1.0f);
		noteinfos[i].Rotate_line = glm::mat4(1.0f);
		noteinfos[i].noteline = 0;
		noteinfos[i].playline = 0;
		noteinfos[i].Trigger = GL_FALSE;
		noteinfos[i].Processed = GL_FALSE;
	}

	FMOD_Channel_Stop(bgc);
	FMOD_Sound_Release(soul);
	FMOD_Sound_Release(animal);
	FMOD_Sound_Release(sea);
	FMOD_System_Close(soundsystem);
	FMOD_System_Release(soundsystem);
}

void Play_state::handle_events(Event evnt) {
	switch (evnt.type) {
	case KEYBOARD:
	{
		switch (evnt.key) {
		case 'a':
			if (!IspressedA) {
				std::cout << "AAA" << std::endl;
				process_note(0);
			}
			break;
		case 's':
			if (!IspressedS) {
				std::cout << "SSS" << std::endl;
				process_note(1);
			}
			break;
		case 'd':
			if (!IspressedD) {
				std::cout << "DDD" << std::endl;
				process_note(2);
			}
			break;
		case 'f':
			if (!IspressedF) {
				std::cout << "FFF" << std::endl;
				process_note(3);
			}
			break;
		case 13:
			break;
		case 27:
			if (!Turning) {
				state = 2;
				mode = 0;
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

				if (Warning) {
					Warning = GL_FALSE;
				}
			}
			break;
		case GLUT_KEY_RIGHT:
			if (selected_num == 0) {
				selected_num = 1;
				Turning = 1;

				if (Warning) {
					Warning = GL_FALSE;
				}
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

void Play_state::update() {
	switch (state)
	{
	case 0:
		brightness += Get_Game_Framework().get_frame_time();
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
			FMOD_Channel_SetVolume(bgc, 1.0 * Get_Option_state().Get_Volume() / 100.0); //--- 볼륨 조절하기

			Trans_playtime = glm::mat4(1.0f);
		}
		break;
	case 1:
		// turn //
		if (Turning == -1) {
			camera_radian -= Get_Game_Framework().get_frame_time() * TURN_SPEED;

			if (camera_radian <= 0.0) {
				camera_radian = 0.0;
				Turning = 0;
			}
		}
		else if (Turning == 1) {
			camera_radian += Get_Game_Framework().get_frame_time() * TURN_SPEED;

			if (camera_radian >= 30.0) {
				camera_radian = 30, 0;
				Turning = 0;
			}
		}

		// time
		Trans_playtime = glm::mat4(1.0f);
		play_time = (float)(clock() - start_time) / 100.0;
		Trans_playtime = glm::translate(Trans_playtime, glm::vec3(0.0, 0.0, play_time));

		// end
		if ((end_time != 0) && (clock() - end_time >= 5000)) {
			state = 2;
			next_state = &Get_End_state();
		}
		break;
	case 2:
		brightness -= Get_Game_Framework().get_frame_time();

		if (brightness <= 0.0) {
			if (next_state != nullptr) {
				if(mode < 0)
					Get_Game_Framework().change_state(next_state);
				else
					Get_Game_Framework().change_state(next_state, mode);
			}
			else {
				exit();
			}
		}
		break;
	default:
		cout << "state가 start, run, end 상태가 아닌 다른상태가 되는 오류 발생" << endl;
		break;
	}
}

void Play_state::draw() {
	glViewport(0, 0, glutGet(0x0066), glutGet(0x0067));
	// init //
	InitBuffer();

	//--- 변경된 배경색 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 TR = glm::mat4(1.0f);

	// program //
	glUseProgram(shader_program);

	// camera //

	glm::vec3 cameraPos = glm::vec3(camera_x, camera_y, camera_z); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(camera_x, camera_y, camera_z - 1.0); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 TR_O = glm::mat4(1.0f);
	glm::mat4 TR_P = glm::mat4(1.0f);
	TR_O = glm::translate(TR_O, glm::vec3(-camera_x, -camera_y, -camera_z));
	TR_P = glm::translate(TR_P, glm::vec3(camera_x, camera_y, camera_z));

	glm::mat4 Rotate_Camera = glm::mat4(1.0f);
	Rotate_Camera = glm::rotate(Rotate_Camera, glm::radians(camera_radian), glm::vec3(0.0, 1.0, 0.0));

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	//view = TR_P * Rotate_Camera * TR_O * view;
	view = view * Rotate_Camera;

	unsigned int viewLocation = glGetUniformLocation(shader_program, "view"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// projection //

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, MAX_PROJECT_Z);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0)); //--- 공간을 약간 뒤로 미뤄줌
	unsigned int projectionLocation = glGetUniformLocation(shader_program, "proj"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	// light //

	unsigned int lightPosLocation = glGetUniformLocation(shader_program, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 2.0, 0.0);

	int lightColorLocation = glGetUniformLocation(shader_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, brightness, brightness, brightness);

	unsigned int viewPosLocation = glGetUniformLocation(shader_program, "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	float ambientLightLocation = glGetUniformLocation(shader_program, "ambientLight");	// ambientLight~
	glUniform1f(ambientLightLocation, 0.5);

	// default //

	glEnable(GL_DEPTH_TEST);

	unsigned int modelLocation = glGetUniformLocation(shader_program, "model");

	int objColorLocation = glGetUniformLocation(shader_program, "objectColor"); //--- object Color값 전달

	int vectorLocation = glGetUniformLocation(shader_program, "tNormal"); // vector

	unsigned int IsText = glGetUniformLocation(shader_program, "IsText");

	// draw //
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

		if (Warning) {
			switch (selected_num) {
			case 0:
				glUniform3f(lightColorLocation, 0.0, 0.0, brightness);
				break;
			case 1:
				glUniform3f(lightColorLocation, brightness, 0.0, 0.0);
				break;
			default:
				break;
			}
		}
		else {
			glUniform3f(lightColorLocation, brightness, brightness, brightness);
		}

		glUniform3f(objColorLocation, 1.0, 0.0, 1.0);
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

		glUniform3f(lightColorLocation, brightness, brightness, brightness);

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
	if (play_time > 0.0) {
		glBindVertexArray(vao[2]);

		for (int k = draw_notenum; k < max_notenum; ++k) {
			if (noteinfos[k].playline == 0) {
				glUniform3f(objColorLocation, 1.0, 0.0, 0.0);
			}
			else {
				glUniform3f(objColorLocation, 0.0, 0.0, 1.0);
			}

			TR = glm::mat4(1.0f);
			TR = Scale_speed * Trans_playtime * noteinfos[k].Trans_time * TR;

			if (TR[3].z >= NOTE_Z_MIDDLE + TRIGGER_TUM * note_speed) {	// miss
				noteinfos[k].Trigger = GL_FALSE;

				if (!noteinfos[k].Processed) {	// miss
					noteinfos[k].Processed = GL_TRUE;
					combo_num = 0;
					miss_num++;
				}

				draw_notenum = k;

				if ((end_time == 0) && (draw_notenum == max_notenum - 1)) {
					end_time = clock();
				}

				if (Warning) {
					Warning = GL_FALSE;
				}
				continue;
			}
			else if (TR[3].z >= NOTE_Z_MIDDLE - TRIGGER_TUM * note_speed) {	// trigger on
				noteinfos[k].Trigger = GL_TRUE;
			}
			else if (TR[3].z >= MAX_NOTE_Z * note_speed) {
				if (noteinfos[k].playline != selected_num) {
					Warning = GL_TRUE;
				}
				trigger_notenum = k;
			}
			else if (TR[3].z <= MAX_NOTE_Z) {
				break;
			}

			TR = noteinfos[k].Rotate_line * TR_t * TR_r1 * noteinfos[k].Trans_line * TR;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);

			if (!noteinfos[k].Processed) {
				for (int i = 0; i < 6; ++i) {
					GLfloat* tcube_vt = cube_vt[i % 6];
					glm::vec3 nVector = glm::mat3(glm::transpose(glm::inverse(TR))) * glm::vec3(tcube_vt[0], tcube_vt[1], tcube_vt[2]);
					glUniform3f(vectorLocation, nVector.x, nVector.y, nVector.z);

					for (int j = 2 * i; j < 2 * i + 2; ++j) {
						glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &line_l[j]);
					}
				}
			}
		}
	}

	// text //
	glUniform1i(IsText, 1);
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	if (combo_num != 0 && state == 1) {
		RenderString(-0.1f, 0.25f, GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)"COMBO", 1.0f, 0.0f, 0.0f);
		RenderString(-0.1f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)to_string(combo_num).c_str(), 1.0f, 0.0f, 0.0f);
	}

	// another line //
	{
		glViewport(glutGet(0x0066) / 3 * 2, glutGet(0x0067) / 3 * 2, glutGet(0x0066) / 3, glutGet(0x0067) / 3);

		// camera //

		cameraPos = glm::vec3(camera_x, camera_y, camera_z + ANOTHER_Z); //--- 카메라 위치
		cameraDirection = glm::vec3(camera_x, camera_y, camera_z + ANOTHER_Z - 1.0); //--- 카메라 바라보는 방향
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
		view = glm::mat4(1.0f);

		glm::mat4 Rotate_Camera = glm::mat4(1.0f);
		Rotate_Camera = glm::rotate(Rotate_Camera, glm::radians((float)((selected_num + 1) % 2) * 30.0f), glm::vec3(0.0, 1.0, 0.0));

		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		view = view * Rotate_Camera;

		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

		// projection //

		// light //
		glUniform3f(lightPosLocation, 0.0, 2.0, ANOTHER_Z);

		// default //

		// draw //
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		TR = glm::mat4(1.0f);
		TR_r1 = glm::mat4(1.0f);
		TR_t = glm::mat4(1.0f);
		TR_r2 = glm::mat4(1.0f);

		TR_r1 = glm::rotate(TR_r1, glm::radians(20.0f), glm::vec3(1.0, 0.0, 0.0));
		TR_t = glm::translate(TR_t, glm::vec3(0.0, 0.0, camera_z + ANOTHER_Z - 3.0));
		TR_r2 = glm::rotate(TR_r2, glm::radians((float)((selected_num + 1) % 2) * -30.0f), glm::vec3(0.0, 1.0, 0.0));

		TR = TR_r2 * TR_t * TR_r1 * TR;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);

		// play line //
		glBindVertexArray(vao[0]);

		glUniform3f(objColorLocation, 1.0, 0.0, 1.0);
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

		glUniform3f(lightColorLocation, brightness, brightness, brightness);

		glUniform3f(objColorLocation, 1.0, 1.0, 0.0);

		for (int i = 0; i < 6; ++i) {
			GLfloat* tcube_vt = cube_vt[i % 6];
			glm::vec3 nVector = glm::mat3(glm::transpose(glm::inverse(TR))) * glm::vec3(tcube_vt[0], tcube_vt[1], tcube_vt[2]);
			glUniform3f(vectorLocation, nVector.x, nVector.y, nVector.z);

			for (int j = 2 * i; j < 2 * i + 2; ++j) {
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &line_l[j]);
			}
		}

		// note //
		if (play_time > 0.0) {
			glBindVertexArray(vao[2]);

			for (int k = draw_notenum; k < max_notenum; ++k) {
				if (noteinfos[k].playline == 0) {
					glUniform3f(objColorLocation, 1.0, 0.0, 0.0);
				}
				else {
					glUniform3f(objColorLocation, 0.0, 0.0, 1.0);
				}

				TR = glm::mat4(1.0f);
				TR = Scale_speed * Trans_playtime * noteinfos[k].Trans_time * TR;

				if (TR[3].z <= MAX_NOTE_Z) {
					break;
				}

				TR = noteinfos[k].Rotate_line * TR_t * TR_r1 * noteinfos[k].Trans_line * TR;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);

				if (!noteinfos[k].Processed && noteinfos[k].playline != selected_num) {
					for (int i = 0; i < 6; ++i) {
						GLfloat* tcube_vt = cube_vt[i % 6];
						glm::vec3 nVector = glm::mat3(glm::transpose(glm::inverse(TR))) * glm::vec3(tcube_vt[0], tcube_vt[1], tcube_vt[2]);
						glUniform3f(vectorLocation, nVector.x, nVector.y, nVector.z);

						for (int j = 2 * i; j < 2 * i + 2; ++j) {
							glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &line_l[j]);
						}
					}
				}
			}
		}
	}

	glutSwapBuffers();
}

void Play_state::GenBuffer() {
	glGenVertexArrays(3, vao);
	glGenBuffers(3, vbo);
}

void Play_state::InitBuffer() {
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

void Play_state::read_file() {
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
		std::cout << "파일 불러오기 실패" << std::endl;
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
		noteinfos[now_notenum].noteline = note_line;
		noteinfos[now_notenum].playline = play_line;

		now_notenum++;

		fscanf(fp, "%f", &note_time);
	}

	max_notenum = now_notenum;

	std::cout << "파일 불러오기 완료" << std::endl;

	fclose(fp);
}

void Play_state::process_note(GLint key) {
	for (int i = draw_notenum; i < trigger_notenum + 1; ++i) {
		if (noteinfos[i].Trigger && !noteinfos[i].Processed) {
			if ((noteinfos[i].noteline == key) && (noteinfos[i].playline == selected_num)) {
				noteinfos[i].Processed = GL_TRUE;

				good_num++;
				combo_num++;
				return;
			}
		}
	}
}

void Play_state::draw_another_line() {
	glViewport(600, 450, 200, 150);

	//--- 변경된 배경색 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 TR = glm::mat4(1.0f);

	// program //
	glUseProgram(shader_program);

	// camera //

	glm::vec3 cameraPos = glm::vec3(camera_x, camera_y, camera_z); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(camera_x, camera_y, camera_z - 1.0); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 TR_O = glm::mat4(1.0f);
	glm::mat4 TR_P = glm::mat4(1.0f);
	TR_O = glm::translate(TR_O, glm::vec3(-camera_x, -camera_y, -camera_z));
	TR_P = glm::translate(TR_P, glm::vec3(camera_x, camera_y, camera_z));

	glm::mat4 Rotate_Camera = glm::mat4(1.0f);
	Rotate_Camera = glm::rotate(Rotate_Camera, glm::radians((float)((selected_num + 1) % 2) * 30.0f), glm::vec3(0.0, 1.0, 0.0));

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	//view = TR_P * Rotate_Camera * TR_O * view;
	view = view * Rotate_Camera;

	unsigned int viewLocation = glGetUniformLocation(shader_program, "view"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// projection //

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 20.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0)); //--- 공간을 약간 뒤로 미뤄줌
	unsigned int projectionLocation = glGetUniformLocation(shader_program, "proj"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	// light //

	unsigned int lightPosLocation = glGetUniformLocation(shader_program, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 2.0, 0.0);

	int lightColorLocation = glGetUniformLocation(shader_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, brightness, brightness, brightness);

	unsigned int viewPosLocation = glGetUniformLocation(shader_program, "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	float ambientLightLocation = glGetUniformLocation(shader_program, "ambientLight");	// ambientLight~
	glUniform1f(ambientLightLocation, 0.5);

	// default //

	glDisable(GL_DEPTH_TEST);

	unsigned int modelLocation = glGetUniformLocation(shader_program, "model");

	int objColorLocation = glGetUniformLocation(shader_program, "objectColor"); //--- object Color값 전달

	int vectorLocation = glGetUniformLocation(shader_program, "tNormal"); // vector

	unsigned int IsText = glGetUniformLocation(shader_program, "IsText");

	// draw //
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

		glUniform3f(objColorLocation, 1.0, 0.0, 1.0);
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

		glUniform3f(lightColorLocation, brightness, brightness, brightness);

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
	if (play_time > 0.0) {
		glBindVertexArray(vao[2]);

		for (int k = draw_notenum; k < max_notenum; ++k) {
			if (noteinfos[k].playline == 0) {
				glUniform3f(objColorLocation, 1.0, 0.0, 0.0);
			}
			else {
				glUniform3f(objColorLocation, 0.0, 0.0, 1.0);
			}

			TR = glm::mat4(1.0f);
			TR = Trans_playtime * noteinfos[k].Trans_time * TR;
			
			if (TR[3].z <= -10.0) {
				break;
			}

			TR = noteinfos[k].Rotate_line * TR_t * TR_r1 * noteinfos[k].Trans_line * Scale_speed * TR;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);

			if (!noteinfos[k].Processed) {
				for (int i = 0; i < 6; ++i) {
					GLfloat* tcube_vt = cube_vt[i % 6];
					glm::vec3 nVector = glm::mat3(glm::transpose(glm::inverse(TR))) * glm::vec3(tcube_vt[0], tcube_vt[1], tcube_vt[2]);
					glUniform3f(vectorLocation, nVector.x, nVector.y, nVector.z);

					for (int j = 2 * i; j < 2 * i + 2; ++j) {
						glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &line_l[j]);
					}
				}
			}
		}
	}
}
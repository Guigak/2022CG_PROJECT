#define _CRT_SECURE_NO_WARNINGS

#include "Made_state.h"
#include "select_made_state.h"

Made_state made_state;

Made_state& Get_Made_state() { return made_state; }

void Made_state::enter(GLuint program, GLuint* a, GLuint* b, GLint s) {
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
	FMOD_System_CreateSound(soundsystem, "Insta_Beat_Vixens.mp3", FMOD_LOOP_OFF, 0, &insta);
	FMOD_System_CreateSound(soundsystem, "Kiss_The_Heavens.mp3", FMOD_LOOP_OFF, 0, &kiss);

	Soundplaying = GL_FALSE;

	selected_song = s;

	note_num = 0;

	IspressedA = GL_FALSE;
	IspressedS = GL_FALSE;
	IspressedD = GL_FALSE;
	IspressedF = GL_FALSE;

	//GenBuffer();
	InitBuffer();
	state = 0;
	next_state = nullptr;

	//
	switch (selected_song) {
	case 0:
		FMOD_System_PlaySound(soundsystem, soul, NULL, 0, &bgc);
		break;
	case 1:
		FMOD_System_PlaySound(soundsystem, insta, NULL, 0, &bgc);
		break;
	case 2:
		FMOD_System_PlaySound(soundsystem, kiss, NULL, 0, &bgc);
		break;
	default:
		break;
	}
}

void Made_state::pause() {

}

void Made_state::resume() {

}

void Made_state::exit() {
	FMOD_Channel_Stop(bgc);
	FMOD_Sound_Release(soul);
	FMOD_Sound_Release(insta);
	FMOD_Sound_Release(kiss);
	FMOD_System_Close(soundsystem);
	FMOD_System_Release(soundsystem);
}

void Made_state::handle_events(Event evnt) {
	switch (evnt.type) {
	case KEYBOARD:
	{
		switch (evnt.key) {
		case 'a':
			if (!IspressedA) {
				notes[note_num].time = clock() - start_time;
				notes[note_num].noteline = 0;
				notes[note_num].playline = selected_num;
				note_num++;

				IspressedA = GL_TRUE;
			}
			break;
		case 's':
			if (!IspressedS) {
				notes[note_num].time = clock() - start_time;
				notes[note_num].noteline = 1;
				notes[note_num].playline = selected_num;
				note_num++;

				IspressedS = GL_TRUE;
			}
			break;
		case 'd':
			if (!IspressedD) {
				notes[note_num].time = clock() - start_time;
				notes[note_num].noteline = 2;
				notes[note_num].playline = selected_num;
				note_num++;

				IspressedD = GL_TRUE;
			}
			break;
		case 'f':
			if (!IspressedF) {
				notes[note_num].time = clock() - start_time;
				notes[note_num].noteline = 3;
				notes[note_num].playline = selected_num;
				note_num++;

				IspressedF = GL_TRUE;
			}
			break;
		case 13:
			if (!Iswrited) {
				write_file();
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

void Made_state::update() {
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
				FMOD_System_PlaySound(soundsystem, insta, NULL, 0, &bgc);
				break;
			case 2:
				FMOD_System_PlaySound(soundsystem, kiss, NULL, 0, &bgc);
				break;
			default:
				break;
			}

			start_time = clock();
			FMOD_Channel_SetVolume(bgc, 0.25);
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
		break;
	case 2:
		brightness -= Get_Game_Framework().get_frame_time() / 2;
		if (brightness <= 0.0) {
			if (next_state != nullptr) {
				Get_Game_Framework().change_state(next_state, 0);
			}
			else {
				exit();
			}
		}
		break;
	default:
		cout << "state�� start, run, end ���°� �ƴ� �ٸ����°� �Ǵ� ���� �߻�" << endl;
		break;
	}
}

void Made_state::draw() {
	// init //
	InitBuffer();

	//--- ����� ���� ����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 TR = glm::mat4(1.0f);

	// program //
	glUseProgram(shader_program);

	// camera //

	glm::vec3 cameraPos = glm::vec3(camera_x, camera_y, camera_z); //--- ī�޶� ��ġ
	glm::vec3 cameraDirection = glm::vec3(camera_x, camera_y, camera_z - 1.0); //--- ī�޶� �ٶ󺸴� ����
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
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

	unsigned int viewLocation = glGetUniformLocation(shader_program, "view"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// projection //

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 15.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0)); //--- ������ �ణ �ڷ� �̷���
	unsigned int projectionLocation = glGetUniformLocation(shader_program, "proj"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	// light //

	unsigned int lightPosLocation = glGetUniformLocation(shader_program, "lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 2.0, 0.0);

	int lightColorLocation = glGetUniformLocation(shader_program, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	glUniform3f(lightColorLocation, brightness, brightness, brightness);

	unsigned int viewPosLocation = glGetUniformLocation(shader_program, "viewPos"); //--- viewPos �� ����: ī�޶� ��ġ
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	float ambientLightLocation = glGetUniformLocation(shader_program, "ambientLight");	// ambientLight~
	glUniform1f(ambientLightLocation, 0.5);

	// default //

	glEnable(GL_DEPTH_TEST);

	unsigned int modelLocation = glGetUniformLocation(shader_program, "model");

	int objColorLocation = glGetUniformLocation(shader_program, "objectColor"); //--- object Color�� ����

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

	glutSwapBuffers();
}

void Made_state::GenBuffer() {
	glGenVertexArrays(3, vao);
	glGenBuffers(3, vbo);
}

void Made_state::InitBuffer() {
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
}

void Made_state::write_file() {
	FILE* fp;

	switch (selected_song) {
	case 0:
		fp = fopen("soul.txt", "w");
		break;
	case 1:
		fp = fopen("insta.txt", "w");
		break;
	case 2:
		fp = fopen("kiss.txt", "w");
		break;
	default:
		fp = fopen("error.txt", "w");
		break;
	}

	if (fp == NULL) {
		std::cout << "���� ���� ����" << std::endl;
		return;
	}

	GLint now_notenum = 0;

	while (notes[now_notenum].time != 0) {
		GLdouble now_notetime = (GLdouble)notes[now_notenum].time / 100.0;
		GLint now_noteline = notes[now_notenum].noteline;
		GLint now_playline = notes[now_notenum].playline;

		fprintf(fp, "%.2f %d %d\n", now_notetime, now_noteline, now_playline);

		now_notenum++;
	}

	fprintf(fp, "%.2f", 0.00f);

	std::cout << "���� ���� �Ϸ�" << std::endl;
	
	fclose(fp);
}


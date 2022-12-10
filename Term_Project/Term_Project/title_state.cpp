#include "title_state.h"
#include "select_made_state.h"
#include "option_state.h"
Title_state title_state;

Title_state& Get_Title_state() { return title_state; }

void Title_state::enter(GLuint program, GLuint* a, GLuint* b) {
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
	state = 0;
	Made = 0;
	next_state = nullptr;

	mode = -1;

	//GenBuffer();
	InitBuffer();

	
}

void Title_state::pause() {

}

void Title_state::resume() {

}

void Title_state::exit() {

}

void Title_state::handle_events(Event evnt) {
	if (state != 1)
		return;
	switch (evnt.type) {
	case KEYBOARD:
	{
		switch (evnt.key) {
		case 'q':
		case 'Q':
			Get_Game_Framework().quit();
			break;
		case 13:
		{
			switch (selected_num) {
			case 0:
				mode = 0;
				next_state = &Get_Select_Made_state();
				break;
			case 1:
				next_state = &Get_Option_state();
				break;
			case 2:
				mode = 1;
				next_state = &Get_Select_Made_state();
				break;
			case 3:
				mode = 2;
				next_state = &Get_Select_Made_state();
				break;
			default:
				break;
			}
			state = 2;
		}
		break;
		case 'm':
			if (Made == 0) {
				Made++;
			}
			else {
				Made = 0;
			}
			break;
		case 'a':
			if (Made == 1) {
				Made++;
			}
			else {
				Made = 0;
			}
			break;
		case 'd':
			if (Made == 2) {
				Made++;
			}
			else {
				Made = 0;
			}
			break;
		case 'e':
			if (Made == 3) {
				max_selnum = 3;
			}
			else {
				Made = 0;
			}
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
			if (camera_radian != 0) {
				Turning = -1;
			}
			break;
		case GLUT_KEY_RIGHT:
			if (camera_radian != max_selnum * TUM_RADIAN) {
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

void Title_state::update() {
	switch (state)
	{
	case 0:
		brightness += Get_Game_Framework().get_frame_time() / 2;
		if (brightness >= 1.0) {
			brightness = 1.0;
			state++;
		}
		break;
	case 1:
		// turn //
		if (Turning == -1) {
			camera_radian -= 1;

			if (camera_radian % TUM_RADIAN == 0) {
				Turning = 0;
				selected_num = camera_radian / TUM_RADIAN;
			}
		}
		else if (Turning == 1) {
			camera_radian += 1;

			if (camera_radian % TUM_RADIAN == 0) {
				Turning = 0;
				selected_num = camera_radian / TUM_RADIAN;
			}
		}
		break;
	case 2:
		brightness -= Get_Game_Framework().get_frame_time() / 2;
		if (brightness <= 0.0) {
			if (next_state != nullptr) {
				if (mode < 0) {
					Get_Game_Framework().change_state(next_state);
				}
				else {
					Get_Game_Framework().change_state(next_state, mode);
				}
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

void Title_state::draw() {
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
	Rotate_Camera = glm::rotate(Rotate_Camera, glm::radians((float)camera_radian * 3.0f), glm::vec3(0.0, 1.0, 0.0));

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	//view = TR_P * Rotate_Camera * TR_O * view;
	view = view * Rotate_Camera;

	unsigned int viewLocation = glGetUniformLocation(shader_program, "view"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// projection //

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 15.0f);
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

	// text //
	if (!Turning && state == 1) {
		glUniform1i(IsText, 1);
		glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

		switch (selected_num) {
		case 0:
			RenderString(-0.1f, -0.25f, GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)"START", 1.0f, 0.0f, 0.0f);
			break;
		case 1:
			RenderString(-0.1f, -0.25f, GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)"OPTION", 1.0f, 0.0f, 0.0f);
			break;
		case 2:
			RenderString(-0.1f, -0.25f, GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)"MADE", 1.0f, 0.0f, 0.0f);
			break;
		case 3:
			RenderString(-0.1f, -0.25f, GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)"TEST", 1.0f, 0.0f, 0.0f);
			break;
		default:
			break;
		}
	}

	glutSwapBuffers();
}

void Title_state::GenBuffer() {
	glGenVertexArrays(3, vao);
	glGenBuffers(3, vbo);
}

void Title_state::InitBuffer() {
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


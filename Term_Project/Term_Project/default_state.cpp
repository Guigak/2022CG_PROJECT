#include "default_state.h"



void Default_state::enter(GLuint program) {
	shader_program = program;

	camera_x = 0.0;
	camera_y = 0.0;
	camera_z = 1.0;

	GenBuffer();
	InitBuffer();
}

void Default_state::pause() {

}

void Default_state::resume() {

}

void Default_state::exit() {
	delete[] vao;
	delete[] vbo;
}

void Default_state::handle_events(Event evnt) {
	std::cout << "handle events" << std::endl;
}

void Default_state::update() {
	//std::cout << "update" << std::endl;
}

void Default_state::draw() {
	//std::cout << "draw" << std::endl;

	// init //
	InitBuffer();

	//--- 변경된 배경색 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 TR_default = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);

	// program //
	glUseProgram(shader_program);

	// camera //

	glm::vec3 cameraPos = glm::vec3(camera_x, camera_y, camera_z); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(shader_program, "view"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// projection //

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0)); //--- 공간을 약간 뒤로 미뤄줌
	unsigned int projectionLocation = glGetUniformLocation(shader_program, "proj"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	// light //

	unsigned int lightPosLocation = glGetUniformLocation(shader_program, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 2.0, 0.0);

	int lightColorLocation = glGetUniformLocation(shader_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 0.7, 0.7, 0.7);

	unsigned int viewPosLocation = glGetUniformLocation(shader_program, "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	float ambientLightLocation = glGetUniformLocation(shader_program, "ambientLight");	// ambientLight~
	glUniform1f(ambientLightLocation, 0.5);

	// default //

	glEnable(GL_DEPTH_TEST);

	unsigned int modelLocation = glGetUniformLocation(shader_program, "model");
	TR = TR_default;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);

	int objColorLocation = glGetUniformLocation(shader_program, "objectColor"); //--- object Color값 전달
	glUniform3f(objColorLocation, 0.0, 0.5, 0.0);

	int vectorLocation = glGetUniformLocation(shader_program, "tNormal"); // vector

	// play line //

	glBindVertexArray(vao[0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < 12; ++i) {
		GLfloat* tcube_vt = cube_vt[i % 6];
		glm::vec3 nVector = glm::mat3(glm::transpose(glm::inverse(glm::mat4(1.0f)))) * glm::vec3(tcube_vt[0], tcube_vt[1], tcube_vt[2]);
		glUniform3f(vectorLocation, nVector.x, nVector.y, nVector.z);

		for (int j = 2 * i; j < 2 * i + 2; ++j) {
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &line_l[j]);
		}
	}

	//
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);

	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
	glUniform1f(ambientLightLocation, 1.0);

	GLUquadricObj* ball0;

	ball0 = gluNewQuadric();
	gluSphere(ball0, 0.05, 10, 10);

	gluDeleteQuadric(ball0);

	glutSwapBuffers();
}

void Default_state::GenBuffer() {
	glGenVertexArrays(3, vao);
	glGenBuffers(3, vbo);
}

void Default_state::InitBuffer() {
	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_v), line_v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}
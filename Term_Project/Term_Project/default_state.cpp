#include "default_state.h"



void Default_state::enter(GLuint* program, GLuint* a, GLuint* b) {
	shader_program = program;
	vao = a;
	vbo = b;

	camera_x = 0.0;
	camera_y = 0.0;
	camera_z = 1.0;

	InitBuffer();
}

void Default_state::pause() {

}

void Default_state::resume() {

}

void Default_state::exit() {

}

void Default_state::handle_events(Event evnt) {
	std::cout << "handle events" << std::endl;
}

void Default_state::update() {
	std::cout << "update" << std::endl;
}

void Default_state::draw() {
	std::cout << "draw" << std::endl;

	// program //
	glUseProgram(*shader_program);

	// camera //
	glm::vec3 cameraPos = glm::vec3(camera_x, camera_y, camera_z); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(*shader_program, "view"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// projection //
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0)); //--- 공간을 약간 뒤로 미뤄줌
	unsigned int projectionLocation = glGetUniformLocation(*shader_program, "proj"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	// default //
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	unsigned int modelLocation = glGetUniformLocation(*shader_program, "model");
	glm::mat4 TR = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);

	int objColorLocation = glGetUniformLocation(*shader_program, "color"); //--- object Color값 전달
	glUniform3f(objColorLocation, 0.0, 0.5, 0.0);

	// play line //

	glBindVertexArray(vao[0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < 12; ++i) {
		for (int j = 2 * i; j < 2 * i + 2; ++j) {
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &line_l[j]);
		}
	}
}

void Default_state::InitBuffer() {
	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_v), line_v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}
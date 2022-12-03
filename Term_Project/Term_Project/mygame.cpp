#define _CRT_SECURE_NO_WARNINGS

// include //
#include <iostream>
#include <cstdio>
#include <random>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "total.h"



// define //



// function (declaration) //

// useful
char* filetobuf(const char* file);
void checkCompileErrors(unsigned int shader, std::string type);
double MouseToWindow_X(int window_width, int x);
double MouseToWindow_Y(int window_height, int y);

// glut
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int, int, int, int);
GLvoid Motion(int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Keyboard_Up(unsigned char key, int x, int y);
GLvoid Special(int, int, int);
GLvoid Special_Up(int, int, int);
GLvoid Timer(int value);

// shader
void InitShader();
void InitBuffer();
void make_fragmentShader();
void make_vertexShader();

// state
void Initvalue();

// Um.. yeah..
//bool is_divide_pt(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22);
//bool is_cross_pt(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22);
//void Draw(bool);
//bool collide(float left_a, float bottom_a, float right_a, float top_a, float left_b, float bottom_b, float right_b, float top_b);



// variable //

// shader
GLchar* vertexsource, * fragmentsource; //--- �ҽ��ڵ� ���� ����
GLuint vertexshader, fragmentshader; //--- ���̴� ��ü
GLuint s_program;

// vertex
GLuint vao[3], vbo[3];	// 0 - line / 1 - note / 2 - judge line

// game framework
Game_Framework game_framework;

// state
State* state;

Dummy_state dummy_state;
Default_state default_state;



// main //
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	glGenVertexArrays(3, vao);
	glGenBuffers(3, vbo);
	Initvalue();
	InitBuffer();

	// loop
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(Keyboard_Up);
	glutSpecialFunc(Special);
	glutSpecialUpFunc(Special_Up);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMotionFunc(Motion);
	glutMainLoop();
}



// function (definition) //

// useful
char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}

void checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

double MouseToWindow_X(int window_width, int x) {
	double re_x = x;
	re_x *= (2 / (double)window_width);
	return re_x - 1.0;
}

double MouseToWindow_Y(int window_height, int y) {
	double re_y = y;
	re_y *= -(2 / (double)window_height);
	return re_y + 1.0;
}

// glut
GLvoid drawScene()
{
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	//glClearColor(0.1, 0.1, 0.1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// program //
	glUseProgram(s_program);

	// camera //
	glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 2.0); //--- ī�޶� ��ġ
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ����
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(s_program, "view"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// projection //
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0)); //--- ������ �ణ �ڷ� �̷���
	unsigned int projectionLocation = glGetUniformLocation(s_program, "proj"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	// default //
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	unsigned int modelLocation = glGetUniformLocation(s_program, "model");
	glm::mat4 TR = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);

	int objColorLocation = glGetUniformLocation(s_program, "color"); //--- object Color�� ����
	glUniform3f(objColorLocation, 0.0, 0.5, 0.0);

	// play line //

	glBindVertexArray(vao[0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < 12; ++i) {
		for (int j = 2 * i; j < 2 * i + 2; ++j) {
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &line_l[j]);
		}
	}

	//game_framework.draw();

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y) {
	Event evnt(event_type::MOUSE, ' ', button, x, y, state);
	game_framework.handle_events(evnt);
}

GLvoid Motion(int x, int y) {
	Event evnt(MOTION, ' ', 0, x, y, 0);
	game_framework.handle_events(evnt);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	Event evnt(KEYBOARD, key, 0, x, y, 0);
	game_framework.handle_events(evnt);
}

GLvoid Keyboard_Up(unsigned char key, int x, int y) {
	Event evnt(KEYBOARD_UP, key, 0, x, y, 0);
	game_framework.handle_events(evnt);
}

GLvoid Special(int key, int x, int y) {
	Event evnt(SPECIAL, 0, key, x, y, 0);
	game_framework.handle_events(evnt);
}

GLvoid Special_Up(int key, int x, int y) {
	Event evnt(SPECIAL_UP, 0, key, x, y, 0);
	game_framework.handle_events(evnt);
}

GLvoid Timer(int value) {
	game_framework.run();
	glutTimerFunc(1, Timer, 1);
	glutPostRedisplay();
}

// shader
void InitShader()
{
	make_vertexShader(); //--- ���ؽ� ���̴� �����
	make_fragmentShader(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);
	checkCompileErrors(s_program, "PROGRAM");
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

void InitBuffer()
{
	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_v), line_v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexshader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
	}
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentshader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
	}
}

// state
void Initvalue() {
	//state = &dummy_state;
	state = &default_state;
	game_framework.init(state, &s_program, vao, vbo);
	glutTimerFunc(1, Timer, 1);
}
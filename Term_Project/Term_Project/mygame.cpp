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
GLchar* vertexsource, * fragmentsource; //--- 소스코드 저장 변수
GLuint vertexshader, fragmentshader; //--- 세이더 객체
GLuint s_program;

// vertex
GLuint vao[3], vbo[3];	// 0 - line / 1 - note / 2 - judge line

// game framework
Game_Framework game_framework;

// state
State* state;

Dummy_state dummy_state;
Default_state default_state;

//

// camera

GLfloat camera_x = 0.0;
GLfloat camera_y = 0.0;
GLfloat camera_z = 5.0;

// light

GLfloat light_rad = 90.0;
GLfloat light_r = 1.0;



// main //
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example");
	//--- GLEW 초기화하기
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
	game_framework.draw();
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
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
	glutTimerFunc(10, Timer, 1);
	glutPostRedisplay();
}

// shader
void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기
	make_fragmentShader(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);
	checkCompileErrors(s_program, "PROGRAM");
	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program 사용하기
	glUseProgram(s_program);
}

void InitBuffer()
{
	/*glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_v), line_v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);*/
}

void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexshader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
	}
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentshader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
	}
}

// state
void Initvalue() {
	//state = &dummy_state;
	state = &default_state;
	game_framework.init(state, s_program);
	glutTimerFunc(1, Timer, 1);
}
// �ı� ����

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <random>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "total.h"


constexpr double PI = 3.1415926;

char* filetobuf(const char* file);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void checkCompileErrors(unsigned int shader, std::string type);
void InitShader();
void InitBuffer();
void make_fragmentShader();
void make_vertexShader();
GLvoid Mouse(int, int, int, int);
GLvoid Motion(int x, int y);
GLvoid Timer(int value);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Keyboard_Up(unsigned char key, int x, int y);
GLvoid Special(int, int, int);
GLvoid Special_Up(int, int, int);
void Initvalue();
double MouseToWindow_X(int window_width, int x);
double MouseToWindow_Y(int window_height, int y);
bool is_divide_pt(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22);
bool is_cross_pt(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22);
void Draw(bool);
bool collide(float left_a, float bottom_a, float right_a, float top_a, float left_b, float bottom_b, float right_b, float top_b);
// ��������
std::random_device rd;
std::uniform_real_distribution<float> urd(0, 1.0);
std::uniform_real_distribution<float> urd_s(0.1, 0.15);
std::uniform_real_distribution<float> urd_m(0.3, 1.0);
GLchar* vertexsource, * fragmentsource; //--- �ҽ��ڵ� ���� ����
GLuint vertexshader, fragmentshader; //--- ���̴� ��ü
GLuint s_program;

GLuint vao[4], vbo[8], ebo[2];

Game_Framework game_framework;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	glGenVertexArrays(4, vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(8, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(2, ebo);
	Initvalue();
	InitBuffer();
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


Dummy_state dummy;


void Initvalue() {
	State* state;
	state = &dummy;
	game_framework.init(state);
	glutTimerFunc(1, Timer, 1);
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

//--- ���� �����ϰ� ������ �޾ƿ���
void InitBuffer()
{
	glBindVertexArray(vao[0]); //--- VAO�� ���ε��ϱ�






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

//--- ���̴� ���α׷� �����ϱ�
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

//--- �׸��� �ݹ� �Լ�
GLvoid drawScene()
{
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
	glClearColor(0.1, 0.1, 0.1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game_framework.draw();
	//glUseProgram(s_program);


	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}


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

bool is_divide_pt(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22) {
	float f1 = (x12 - x11) * (y21 - y11) - (y12 - y11) * (x21 - x11);
	float f2 = (x12 - x11) * (y22 - y11) - (y12 - y11) * (x22 - x11);
	if (f1 * f2 < 0)
		return true;
	else
		return false;
}

bool is_cross_pt(float x11, float y11, float x12, float y12, float x21, float y21, float x22, float y22) {
	bool b1 = is_divide_pt(x11, y11, x12, y12, x21, y21, x22, y22);
	bool b2 = is_divide_pt(x21, y21, x22, y22, x11, y11, x12, y12);
	if (b1 && b2)
		return true;
}

bool collide(float left_a, float bottom_a, float right_a, float top_a, float left_b, float bottom_b, float right_b, float top_b) {
	if (left_a > right_b) return false;
	if (right_a < left_b) return false;
	if (top_a < bottom_b) return false;
	if (bottom_a > top_b) return false;
	return true;
}

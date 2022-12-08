#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>

// define

#define TIMER_TIME 100

// struct

typedef struct _TPOINT {
	GLfloat x;
	GLfloat y;
} TPOINT;

// useful functions

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

void RenderString(float x, float y, void* font, unsigned char* string, float r, float g, float b)
{
	char* c;

	glColor3f(r, g, b);
	glRasterPos2f(x, y);

	glutBitmapString(font, string);
}

TPOINT Trans_Point(int mx, int my) {
	TPOINT tpt = { 0 };

	GLint width = glutGet(0x0066) / 2;
	GLint height = glutGet(0x0067) / 2;

	tpt.x = (float)(mx - width) / (float)width;
	tpt.y = -(float)(my - height) / (float)height;

	return tpt;
}

// function

void make_vertexShader();
void make_fragmentShader();
void InitBuffer();
void InitShader();

GLvoid drawScene();
GLvoid Reshape(int, int);
GLvoid Keyboard(unsigned char, int, int);

void TimerFunc(int);

void Mouse(int, int, int, int);
void Motion(int, int);

// data

GLboolean Draw_Type = GL_TRUE; // 0 : ��, 1 : ��

GLchar* vertexsource, * fragmentsource; //--- �ҽ��ڵ� ���� ����
GLuint vertexshader, fragmentshader; //--- ���̴� ��ü
GLuint s_program;

// play line //
#define LINE_X_MIDDLE 1.125
#define LINE_X_TUM 0.125
#define LINE_Y_MIDDLE 0.125
#define LINE_Y_TUM 0.0625

GLfloat line_cr = 0.0f;
GLfloat line_cg = 1.0f;
GLfloat line_cb = 0.0f;

GLfloat line_v[] = {	// point, vector
	// 1
	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	0.0, 0.0, 1.0,
	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0,	0.0, 0.0, 1.0,
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0,	0.0, 0.0, 1.0,

	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	0.0, 0.0, 1.0,
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0,	0.0, 0.0, 1.0,
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	0.0, 0.0, 1.0,
	//
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	1.0, 0.0, 0.0,
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0,	1.0, 0.0, 0.0,
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0,	1.0, 0.0, 0.0,

	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	1.0, 0.0, 0.0,
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0,	1.0, 0.0, 0.0,
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	1.0, 0.0, 0.0,
	//
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	0.0, 0.0, -1.0,
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0,	0.0, 0.0, -1.0,
	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0,	0.0, 0.0, -1.0,

	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	0.0, 0.0, -1.0,
	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0,	0.0, 0.0, -1.0,
	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	0.0, 0.0, -1.0,
	//
	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	-1.0, 0.0, 0.0,
	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0,	-1.0, 0.0, 0.0,
	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0,	-1.0, 0.0, 0.0,

	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	-1.0, 0.0, 0.0,
	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0,	-1.0, 0.0, 0.0,
	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	-1.0, 0.0, 0.0,
	//
	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	0.0, 1.0, 0.0,
	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	0.0, 1.0, 0.0,
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	0.0, 1.0, 0.0,

	-LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	0.0, 1.0, 0.0,
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	0.0, 1.0, 0.0,
	-LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	0.0, 1.0, 0.0,
	// 2
	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	0.0, 0.0, 1.0,
	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0,	0.0, 0.0, 1.0,
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0,	0.0, 0.0, 1.0,

	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	0.0, 0.0, 1.0,
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0,	0.0, 0.0, 1.0,
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	0.0, 0.0, 1.0,
	//
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	1.0, 0.0, 0.0,
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0,	1.0, 0.0, 0.0,
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0,	1.0, 0.0, 0.0,

	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	1.0, 0.0, 0.0,
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0,	1.0, 0.0, 0.0,
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	1.0, 0.0, 0.0,
	//
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	0.0, 0.0, -1.0,
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0,	0.0, 0.0, -1.0,
	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0,	0.0, 0.0, -1.0,

	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	0.0, 0.0, -1.0,
	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0,	0.0, 0.0, -1.0,
	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	0.0, 0.0, -1.0,
	//
	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	-1.0, 0.0, 0.0,
	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0,	-1.0, 0.0, 0.0,
	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0,	-1.0, 0.0, 0.0,

	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	-1.0, 0.0, 0.0,
	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0,	-1.0, 0.0, 0.0,
	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	-1.0, 0.0, 0.0,
	//
	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	0.0, 1.0, 0.0,
	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	0.0, 1.0, 0.0,
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	0.0, 1.0, 0.0,

	LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	0.0, 1.0, 0.0,
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0,	0.0, 1.0, 0.0,
	LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0,	0.0, 1.0, 0.0,
};

GLfloat trigger_v[] = {
	//
	-LINE_X_MIDDLE, LINE_Y_MIDDLE, 0.0,								0.0, 0.0, 1.0,
	-LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, 0.0,				0.0, 0.0, 1.0,
	LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, 0.0,					0.0, 0.0, 1.0,

	-LINE_X_MIDDLE, LINE_Y_MIDDLE, 0.0,								0.0, 0.0, 1.0,
	LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, 0.0,					0.0, 0.0, 1.0,
	LINE_X_MIDDLE, LINE_Y_MIDDLE, 0.0,								0.0, 0.0, 1.0,
	//
	LINE_X_MIDDLE, LINE_Y_MIDDLE, 0.0,								1.0, 0.0, 0.0,
	LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, 0.0,					1.0, 0.0, 0.0,
	LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, -LINE_Y_TUM * 3.0,	1.0, 0.0, 0.0,

	LINE_X_MIDDLE, LINE_Y_MIDDLE, 0.0,								1.0, 0.0, 0.0,
	LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, -LINE_Y_TUM * 3.0,	1.0, 0.0, 0.0,
	LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, -LINE_Y_TUM * 3.0,	1.0, 0.0, 0.0,
	//
	LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, -LINE_Y_TUM * 3.0,	0.0, 0.0, -1.0,
	LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, -LINE_Y_TUM * 3.0,	0.0, 0.0, -1.0,
	-LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, -LINE_Y_TUM * 3.0,	0.0, 0.0, -1.0,

	LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, -LINE_Y_TUM * 3.0,	0.0, 0.0, -1.0,
	-LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, -LINE_Y_TUM * 3.0,	0.0, 0.0, -1.0,
	-LINE_X_MIDDLE, LINE_Y_MIDDLE, -LINE_Y_TUM * 3.0,				0.0, 0.0, -1.0,
	//
	-LINE_X_MIDDLE, LINE_Y_MIDDLE, -LINE_Y_TUM * 3.0,				-1.0, 0.0, 0.0,
	-LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, -LINE_Y_TUM * 3.0,	-1.0, 0.0, 0.0,
	-LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, 0.0,				-1.0, 0.0, 0.0,

	-LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, -LINE_Y_TUM * 3.0,	-1.0, 0.0, 0.0,
	-LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, 0.0,				-1.0, 0.0, 0.0,
	-LINE_X_MIDDLE, LINE_Y_MIDDLE, 0.0,								-1.0, 0.0, 0.0,
	//
	-LINE_X_MIDDLE, LINE_Y_MIDDLE, -LINE_Y_TUM * 3.0,				0.0, 1.0, 0.0,
	-LINE_X_MIDDLE, LINE_Y_MIDDLE, 0.0,								0.0, 1.0, 0.0,
	LINE_X_MIDDLE, LINE_Y_MIDDLE, 0.0,								0.0, 1.0, 0.0,

	-LINE_X_MIDDLE, LINE_Y_MIDDLE, -LINE_Y_TUM * 3.0,				0.0, 1.0, 0.0,
	LINE_X_MIDDLE, LINE_Y_MIDDLE, 0.0,								0.0, 1.0, 0.0,
	LINE_X_MIDDLE, LINE_Y_MIDDLE, -LINE_Y_TUM * 3.0,				0.0, 1.0, 0.0,
};

GLuint line_a, line_b;
GLuint trigger_a, trigger_b;

GLboolean First = GL_TRUE;

// Matrix

glm::mat4 Matrix_line = glm::mat4(1.0f);

// command

// camera

GLfloat camera_x = 0.0;
GLfloat camera_y = 1.0;
GLfloat camera_z = 3.0;

// light

GLfloat light_rad = 90.0;
GLfloat light_r = 1.0;

// main

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 750);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(TIMER_TIME, TimerFunc, 1);
	glutMainLoop();
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
		return;
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
		return;
	}
}

void InitBuffer()
{
	if (First) {
		glGenBuffers(1, &line_b);
	}

	if (First) {
		glGenVertexArrays(1, &line_a);
	}
	glBindVertexArray(line_a);

	glBindBuffer(GL_ARRAY_BUFFER, line_b);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_v), line_v, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	if (First) {
		glGenBuffers(1, &trigger_b);
	}

	if (First) {
		glGenVertexArrays(1, &trigger_a);
	}
	glBindVertexArray(trigger_a);

	glBindBuffer(GL_ARRAY_BUFFER, trigger_b);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trigger_v), trigger_v, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	First = GL_FALSE;
}

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

GLvoid drawScene()
{
	//--- ����� ���� ����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(s_program);

	glm::mat4 TR_default = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);

	// Transform

	Matrix_line = glm::mat4(1.0f);
	Matrix_line = glm::rotate(Matrix_line, glm::radians(20.0f), glm::vec3(1.0, 0.0, 0.0));

	TR_default = Matrix_line;

	// hide

	glEnable(GL_DEPTH_TEST);

	// camera

	glm::vec3 cameraPos = glm::vec3(camera_x, camera_y, camera_z); //--- ī�޶� ��ġ
	glm::vec3 cameraDirection = glm::vec3(camera_x, camera_y, camera_z - 1.0); //--- ī�޶� �ٶ󺸴� ����
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLocation = glGetUniformLocation(s_program, "view"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// projection

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 15.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0)); //--- ������ �ణ �ڷ� �̷���
	unsigned int projectionLocation = glGetUniformLocation(s_program, "proj"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	// light

	unsigned int lightPosLocation = glGetUniformLocation(s_program, "lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, light_r * cos(glm::radians(light_rad)), 2.0, light_r * sin(glm::radians(light_rad)));

	int lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	//glUniform3f(lightColorLocation, 2.0, 2.0, 2.0);
	//glUniform3f(lightColorLocation, 1.5, 1.5, 1.5);
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	//glUniform3f(lightColorLocation, 0.7, 0.7, 0.7);

	unsigned int viewPosLocation = glGetUniformLocation(s_program, "viewPos"); //--- viewPos �� ����: ī�޶� ��ġ
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

	float ambientLightLocation = glGetUniformLocation(s_program, "ambientLight");	// ambientLight~
	glUniform1f(ambientLightLocation, 0.5);

	// default

	unsigned int modelLocation = glGetUniformLocation(s_program, "model");
	TR = TR_default;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);

	int objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color�� ����
	glUniform3f(objColorLocation, 0.0, 0.5, 0.0);

	unsigned int IsText = glGetUniformLocation(s_program, "IsText");
	glUniform1i(IsText, 0);

	// play line
	glBindVertexArray(line_a);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUniform3f(objColorLocation, line_cr, line_cg, line_cb);

	glDrawArrays(GL_TRIANGLES, 0, 72);

	// trigger line
	glBindVertexArray(trigger_a);

	glUniform3f(objColorLocation, 1.0, 1.0, 0.0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// light
	//glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	//glm::mat4 Trans_L = glm::mat4(1.0f);
	//Trans_L = glm::translate(Trans_L, glm::vec3(light_r * cos(glm::radians(light_rad)), 0.0, light_r * sin(glm::radians(light_rad))));

	//TR = Trans_L;
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);

	//glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
	//glUniform1f(ambientLightLocation, 1.0);

	//GLUquadricObj* ball0;

	//ball0 = gluNewQuadric();
	//gluSphere(ball0, 0.05, 10, 10);

	//gluDeleteQuadric(ball0);

	////
	//TR = glm::mat4(1.0f);
	//TR = glm::rotate(TR, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));;
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &TR[0][0]);

	//glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	//GLUquadricObj* circle;

	//circle = gluNewQuadric();
	//gluQuadricDrawStyle(circle, GLU_FILL);
	//gluDisk(circle, light_r - 0.01, light_r + 0.01, 20, 20);

	//gluDeleteQuadric(circle);

	// text
	glUniform1i(IsText, 1);
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);

	RenderString(-0.1f, -0.25f, GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)"START", 1.0f, 0.0f, 0.0f);

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
	case 'Q':
		exit(EXIT_FAILURE);
		break;
	case '[':
		light_rad += 5.0;
		break;
	case ']':
		light_rad -= 5.0;
		break;
	case '-':
		if (light_r > 0.25) {
			light_r -= 0.01;
		}
		break;
	case '+':
		if (light_r < 2.0) {
			light_r += 0.01;
		}
		break;
	default:
		break;
	}
	//glutPostRedisplay();
}

void TimerFunc(int value) {

	InitBuffer();
	glutPostRedisplay();
	glutTimerFunc(TIMER_TIME, TimerFunc, 1);
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}

	glutPostRedisplay();
}

void Motion(int x, int y) {

	glutPostRedisplay();
}

// vertex shader

//#version 330 core
//
//layout(location = 0) in vec3 vPos;
//layout(location = 1) in vec3 vNormal;
//
//out vec3 FragPos; //--- ��ü�� ��ġ���� �����׸�Ʈ ���̴��� ������.
//out vec3 Normal; //--- ��ְ��� �����׸�Ʈ ���̴��� ������.
//
//uniform mat4 model; //--- �𵨸� ��ȯ��
//uniform mat4 view; //--- ���� ��ȯ��
//uniform mat4 proj; //--- ���� ��ȯ��
//
//void main()
//{
//	gl_Position = proj * view * model * vec4(vPos, 1.0);
//	FragPos = vec3(model * vec4(vPos, 1.0)); //--- ��ü�� ���� ���� ����� �����׸�Ʈ ���̴����� �Ѵ�. 
//											//--- ���� ��������� �ִ� ���ؽ� ���� �����׸�Ʈ ���̴��� ������.
//	Normal = vNormal; //--- ��ְ��� �����׸�Ʈ ���̴��� ������.
//}


// fragment shader

//#version 330 core
//
//in vec3 FragPos;
//in vec3 Normal;
//
//out vec4 FragColor;
//
//uniform vec3 lightPos;
//uniform vec3 lightColor;
//uniform vec3 objectColor;
//uniform float ambientLight;
//uniform vec3 viewPos;
//uniform vec3 tNormal;
//
//void main() {
//	vec3 ambient = ambientLight * lightColor;
//
//	//vec3 normalVector = normalize(Normal);
//	vec3 normalVector = normalize(tNormal);
//	vec3 lightDir = normalize(lightPos - FragPos);
//	float diffuseLight = max(dot(normalVector, lightDir), 0.0);
//	vec3 diffuse = diffuseLight * lightColor;
//
//	int shininess = 128;
//	vec3 viewDir = normalize(viewPos - FragPos);
//	vec3 reflectDir = reflect(-lightDir, normalVector);
//	float specularLight = max(dot(viewDir, reflectDir), 0.0);
//	specularLight = pow(specularLight, shininess);
//	vec3 specular = specularLight * lightColor;
//
//	vec3 result = (ambient + diffuse + specular) * objectColor;
//	FragColor = vec4(result, 1.0);
//}

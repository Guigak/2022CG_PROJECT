#include "vertices.h"

// define //
#define LINE_X_MIDDLE 1.25
#define LINE_X_TUM 0.25
#define LINE_Y_MIDDLE 0.125
#define LINE_Y_TUM 0.0625

#define TRUGGER_Z -0.01



// play line //
GLfloat line_v[16][3] = {
	{ -LINE_X_MIDDLE,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0 },	// left
	{ -LINE_X_MIDDLE,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0 },
	{ -LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0 },
	{ -LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0 },
	{ -LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0 },
	{ -LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0 },
	{ -LINE_X_MIDDLE,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0 },
	{ -LINE_X_MIDDLE,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0 },
	{ LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0 },	// right
	{ LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0 },
	{ LINE_X_MIDDLE,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0 },
	{ LINE_X_MIDDLE,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0 },
	{ LINE_X_MIDDLE,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0 },
	{ LINE_X_MIDDLE,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0 },
	{ LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	-20.0 },
	{ LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	-20.0 }
};

GLuint line_l[24][3] = {
	{ 0, 1, 2 },	// �ո�
	{ 0, 2, 3 },
	{ 3, 2, 4 },	// ������
	{ 3, 4, 5 },
	{ 5, 4, 6 },	// �޸�
	{ 5, 6, 7 },
	{ 7, 6, 1 },	// �޸�
	{ 7, 1, 0 },
	{ 7, 0, 3 },	// ����
	{ 7, 3, 5 },
	{ 1, 6, 4 },	// �Ʒ���
	{ 1, 4, 2 },
	{ 0 + 8, 1 + 8, 2 + 8 },	// �ո�
	{ 0 + 8, 2 + 8, 3 + 8 },
	{ 3 + 8, 2 + 8, 4 + 8 },	// ������
	{ 3 + 8, 4 + 8, 5 + 8 },
	{ 5 + 8, 4 + 8, 6 + 8 },	// �޸�
	{ 5 + 8, 6 + 8, 7 + 8 },
	{ 7 + 8, 6 + 8, 1 + 8 },	// �޸�
	{ 7 + 8, 1 + 8, 0 + 8 },
	{ 7 + 8, 0 + 8, 3 + 8 },	// ����
	{ 7 + 8, 3 + 8, 5 + 8 },
	{ 1 + 8, 6 + 8, 4 + 8 },	// �Ʒ���
	{ 1 + 8, 4 + 8, 2 + 8 }
};



// trigger rect //
GLfloat trigger_v[8][3] = {
	{ -LINE_X_MIDDLE, LINE_Y_MIDDLE, TRUGGER_Z },
	{ -LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, TRUGGER_Z },
	{ LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, TRUGGER_Z },
	{ LINE_X_MIDDLE, LINE_Y_MIDDLE, TRUGGER_Z },
	{ LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, -LINE_Y_TUM * 3.0 },
	{ LINE_X_MIDDLE, LINE_Y_MIDDLE, -LINE_Y_TUM * 3.0 },
	{ -LINE_X_MIDDLE, LINE_Y_MIDDLE - LINE_Y_TUM, -LINE_Y_TUM * 3.0 },
	{ -LINE_X_MIDDLE, LINE_Y_MIDDLE, -LINE_Y_TUM * 3.0 }
};



// vector //
GLfloat cube_vt[6][3] = {
	{ 0.0, 0.0, 1.0 },	// �ո�
	{ 1.0, 0.0, 0.0 },	// ������
	{ 0.0, 0.0, -1.0 },	// �޸�
	{ -1.0, 0.0, 0.0 },	// �޸�
	{ 0.0, 1.0, 0.0 },	// ����
	{ 0.0, -1.0, 0.0 }	// �Ʒ���
};



// text //
void RenderString(float x, float y, void* font, unsigned char* string, float r, float g, float b) {
	glColor3f(r, g, b);
	glRasterPos2f(x, y);

	glutBitmapString(font, string);
}
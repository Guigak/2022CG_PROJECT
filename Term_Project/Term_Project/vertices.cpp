#include "vertices.h"

// define //
#define LINE_X_MIDDLE 1.25
#define LINE_X_TUM 0.25
#define LINE_Y_MIDDLE 0.125
#define LINE_Y_TUM 0.0625



// play line //
GLfloat line_v[16][3] = {
	{ -LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	10.0 },	// left
	{ -LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	10.0 },
	{ -LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	10.0 },
	{ -LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	10.0 },
	{ -LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0 },
	{ -LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0 },
	{ -LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0 },
	{ -LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0 },
	{ LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	10.0 },	// right
	{ LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	10.0 },
	{ LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	10.0 },
	{ LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	10.0 },
	{ LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0 },
	{ LINE_X_MIDDLE + LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0 },
	{ LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE - LINE_Y_TUM,	0.0 },
	{ LINE_X_MIDDLE - LINE_X_TUM,	LINE_Y_MIDDLE + LINE_Y_TUM,	0.0 }
};

GLuint line_l[24][3] = {
	{ 0, 1, 2 },	// ¾Õ¸é
	{ 0, 2, 3 },
	{ 3, 2, 4 },	// ¿À¸¥¸é
	{ 3, 4, 5 },
	{ 5, 4, 6 },	// µÞ¸é
	{ 5, 6, 7 },
	{ 7, 6, 1 },	// ¿Þ¸é
	{ 7, 1, 0 },
	{ 7, 0, 3 },	// À­¸é
	{ 7, 3, 5 },
	{ 1, 6, 4 },	// ¾Æ·§¸é
	{ 1, 4, 2 },
	{ 0 + 8, 1 + 8, 2 + 8 },	// ¾Õ¸é
	{ 0 + 8, 2 + 8, 3 + 8 },
	{ 3 + 8, 2 + 8, 4 + 8 },	// ¿À¸¥¸é
	{ 3 + 8, 4 + 8, 5 + 8 },
	{ 5 + 8, 4 + 8, 6 + 8 },	// µÞ¸é
	{ 5 + 8, 6 + 8, 7 + 8 },
	{ 7 + 8, 6 + 8, 1 + 8 },	// ¿Þ¸é
	{ 7 + 8, 1 + 8, 0 + 8 },
	{ 7 + 8, 0 + 8, 3 + 8 },	// À­¸é
	{ 7 + 8, 3 + 8, 5 + 8 },
	{ 1 + 8, 6 + 8, 4 + 8 },	// ¾Æ·§¸é
	{ 1 + 8, 4 + 8, 2 + 8 }
};



// vector //
GLfloat cube_vt[6][3] = {
	{ 0.0, 0.0, 1.0 },	// ¾Õ¸é
	{ 1.0, 0.0, 0.0 },	// ¿À¸¥¸é
	{ 0.0, 0.0, -1.0 },	// µÞ¸é
	{ -1.0, 0.0, 0.0 },	// ¿Þ¸é
	{ 0.0, 1.0, 0.0 },	// À­¸é
	{ 0.0, -1.0, 0.0 }	// ¾Æ·§¸é
};
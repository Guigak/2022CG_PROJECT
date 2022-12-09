#pragma once
// include //
#include <gl/glew.h>
#include <gl/freeglut.h>



// play line //
extern GLfloat line_v[16][3];

extern GLuint line_l[24][3];



// trigger rect //
extern GLfloat trigger_v[8][3];



// note //
extern GLfloat note_v[8][3];



// vector //
extern GLfloat cube_vt[6][3];



// text //
void RenderString(float, float, void*, unsigned char*, float, float, float);
#version 330 core

out vec4 FragColor;
in vec3 outColor; //--- 컬러를 fragment shader로 출력

uniform vec3 Color;

void main()
{
    FragColor = vec4(Color, 1.0);
}
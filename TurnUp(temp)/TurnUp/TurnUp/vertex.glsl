#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

out vec3 FragPos; //--- ��ü�� ��ġ���� �����׸�Ʈ ���̴��� ������.
out vec3 Color;
out vec3 Normal; //--- ��ְ��� �����׸�Ʈ ���̴��� ������.

uniform mat4 model; //--- �𵨸� ��ȯ��
uniform mat4 view; //--- ���� ��ȯ��
uniform mat4 proj; //--- ���� ��ȯ��

void main()
{
	gl_Position = proj * view * model * vec4(vPos, 1.0);
	FragPos = vec3(model * vec4(vPos, 1.0)); //--- ��ü�� ���� ���� ����� �����׸�Ʈ ���̴����� �Ѵ�. 
											//--- ���� ��������� �ִ� ���ؽ� ���� �����׸�Ʈ ���̴��� ������.
	Normal = vNormal; //--- ��ְ��� �����׸�Ʈ ���̴��� ������.
}
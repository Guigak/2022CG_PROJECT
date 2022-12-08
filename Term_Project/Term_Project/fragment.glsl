#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientLight;
uniform vec3 viewPos;
uniform vec3 tNormal;

uniform int IsText;

void main() {
	vec3 ambient = ambientLight * lightColor;

	//vec3 normalVector = normalize(Normal);
	vec3 normalVector = normalize(tNormal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuseLight = max(dot(normalVector, lightDir), 0.0);
	vec3 diffuse = diffuseLight * lightColor;

	int shininess = 128;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalVector);
	float specularLight = max(dot(viewDir, reflectDir), 0.0);
	specularLight = pow(specularLight, shininess);
	vec3 specular = specularLight * lightColor;

	vec3 result;

	if (IsText == 0)
		result = (ambient + diffuse + specular) * objectColor;
	else
		result = objectColor;

	FragColor = vec4(result, 1.0);
}

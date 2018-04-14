#version 440 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexColor;

out vec4 fragColor;

void main()
{
	fragColor = vec4(vertexColor, 1.0f);
	gl_Position = vec4(vertexPos, 1.0);
}
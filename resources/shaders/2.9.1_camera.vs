#version 430 core
layout (location = 0) in vec4 pos;
layout (location = 1) in vec2 vertexTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

void main()
{
    gl_Position = projection * view * model * pos;
    texCoord = vertexTexCoord;
}
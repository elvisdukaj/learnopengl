#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 vertexTexCoord;

out vec3 fragColor;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(pos, 1.0);
    fragColor = vertexColor;
    texCoord = vertexTexCoord;
}
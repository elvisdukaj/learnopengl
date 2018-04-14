#version 430 core
out vec4 color;
  
in vec2 texCoord;

uniform sampler2D text;

void main()
{
    color = texture(text, texCoord);
}
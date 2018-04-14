#version 430 core
out vec4 color;
  
in vec3 fragColor;
in vec2 texCoord;

uniform sampler2D text1;
uniform sampler2D text2;

void main()
{
    color = mix(texture(text1, texCoord), texture(text2, texCoord), 0.2);
}
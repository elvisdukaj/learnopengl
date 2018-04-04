#version 430 core
out vec4 FragColor;
  
in vec3 fragColor;
in vec2 texCoord;

uniform sampler2D texSample;

void main()
{
    FragColor = texture(texSample, texCoord);
}
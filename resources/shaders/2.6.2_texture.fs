#version 430 core
out vec4 color;
  
in vec3 fragColor;
in vec2 texCoord;

uniform sampler2D texSample;

void main()
{
    color = texture(texSample, texCoord) * vec4(fragColor, 1.0f);
}
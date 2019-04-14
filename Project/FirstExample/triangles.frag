#version 430 core
in vec3 myColor;
in vec2 texCoord;

out vec4 fColor;

uniform sampler2D texture0;

void main()
{
	fColor = texture(texture0, texCoord);
}
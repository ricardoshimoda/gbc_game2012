#version 430 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec2 vertex_texture;

//out vec4 myColor;
out vec2 texCoord;

uniform mat4 MVP;

void main()
{
	texCoord = vertex_texture;
	//myColor = vec4(vColor, 0);
	gl_Position = MVP * vec4(vPosition,1);
}
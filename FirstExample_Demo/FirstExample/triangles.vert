#version 430 core
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
uniform mat4 Model;

out vec4 myColor;

void main()
{	
	myColor = vColor;
	gl_Position = Model * vPosition;
	
}

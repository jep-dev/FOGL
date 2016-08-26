#version 330 core

precision highp float

layout(location = 0) out vec3 color;
in vec3 vertex;

void main() {
	gl_Position = vec4(vertex, 1.0);
	color = vec3(1.0,1.0,1.0);
}

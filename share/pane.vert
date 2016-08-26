#version 330 core

precision highp float

in vec3 vertex;
layout(location = 0) out vec3 color;

void main() {
	gl_Position = vec4(vertex, 1.0);
	color = vec3(1.0,1.0,1.0);
}

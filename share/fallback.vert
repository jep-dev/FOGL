#version 330 core

precision highp float;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

in vec3 vertex;
out vec4 InColor;

void main() {
	gl_Position = proj * view * model * vec4(vertex, 1.0);
	InColor = vec4(1.0, 1.0, 1.0, 1.0);
}

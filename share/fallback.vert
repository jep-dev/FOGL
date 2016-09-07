#version 330 core

precision highp float;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

in vec3 vertex;
out vec4 InColor;

void main() {
	vec4 pos = vec4(vertex, 1.0);
	gl_Position = proj * view * model * pos;
	InColor = vec4((pos/dot(pos,pos)).xyz, 1.0);
}

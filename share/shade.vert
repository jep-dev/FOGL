#version 330 core

precision highp float;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

in vec3 vertex;
in vec3 normal;
out vec4 InColor;

void main() {
	mat4 vm = view * model;
	gl_Position = proj * vm * vec4(vertex, 1.0);
	vec4 tnorm = vm * vec4(normal, 1.0);
	vec3 half3 = vec3(.5,.5,.5);
	vec3 dotted = dot(tnorm.xyz,vec3(-1,0,0))*half3+half3;
	InColor = vec4(dotted/2+dotted*dotted*dotted,1.0);
}

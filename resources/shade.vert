#version 330 core

precision highp float;

uniform mat4 transform;
uniform mat2 projXY;
uniform mat2 projZW;

in vec4 position;
out vec4 InColor;

void main() {
	vec4 transformed = transform * position;
	gl_Position = vec4(projXY*transformed.xy, projZW*transformed.zw);
	InColor = vec4(normalize(position.xyz) * 0.5 + vec3(0.5, 0.5, 0.5), 1.0);
}

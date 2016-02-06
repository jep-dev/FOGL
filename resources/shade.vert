#version 300 es

precision highp float;

uniform mat4 transform;
uniform mat2 projXY;
uniform mat2 projZW;

in vec3 position;
out vec4 InColor;

void main() {
	vec4 transformed = transform * vec4(position,1.0);
	gl_Position = vec4(projXY*transformed.xy, projZW*transformed.zw);
	InColor = vec4(normalize(position) * 0.5 + vec3(0.5, 0.5, 0.5), 1.0);
}

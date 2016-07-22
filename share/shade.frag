#version 330 core

precision highp float;

in vec4 InColor;
out vec4 FragColor;

void main() {
	FragColor = InColor;
}

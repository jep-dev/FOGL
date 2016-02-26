#version 330 core

precision highp float;

uniform mat4 transform;
uniform mat2 projXY;
uniform mat2 projZW;

in vec3 vertex;
in vec3 normal;
out vec4 InColor;

void main() {
	vec4 position = vec4(vertex, 1.0);
	vec4 transformed = transform * position;
	//vec4 transformedNormal = transform * vec4(normal, 1.0);
	gl_Position = vec4(projXY*transformed.xy, projZW*transformed.zw);
	//InColor = vec4((sin(position.z*800)*0.5+0.5)*vec3(1.0,1.0,1.0),1.0);
	//float u = dot(normalize(vertex), normalize(transformed.xyz));
	//InColor = vec4(u*vec3(1.0,1.0,1.0), 1.0);
	//InColor = vec4(normalize(position.xyz) * 0.5 + vec3(0.5, 0.5, 0.5), 1.0);
	InColor = vec4(dot(normal, vec3(0,1,0))*vec3(.5,.5,.5), 0.0) + vec4(0.5,0.5,0.5,1.0);
}

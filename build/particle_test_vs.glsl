#version 330 core
layout (location=0) in vec3 vertex;
layout (location=1) in vec4 offset;
layout (location=2) in vec4 color;

out vec4 v_color;

void main(){
	gl_Position = vec4(vertex + offset.xyz, 1.0);
	v_color = color;
	gl_PointSize = offset.w;
}
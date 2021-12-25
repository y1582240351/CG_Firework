#version 330 core
in vec4 v_color;

out vec4 outColor;

void main() {
	vec2 temp = gl_PointCoord - vec2(0.5);
	float f = dot(temp, temp);

	if (f > 0.25)
	{
		discard;
	}
	outColor = v_color;
}
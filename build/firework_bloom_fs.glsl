#version 330 core
in vec4 v_color;

// uniform sampler2D sprite_texture;
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 BrightColor;

void main() {
	vec2 temp = gl_PointCoord - vec2(0.5);
	float f = dot(temp, temp);

	if (f > 0.25)
	{
		discard;
	}

	outColor = v_color;

    float brightness = dot(outColor.rgb,vec3(0.2126,0.7152,0.0722));
    if(brightness > 0.1)
        BrightColor = outColor;
    else
        BrightColor = vec4(0.1, 0.1, 0.1, 1.0);
}
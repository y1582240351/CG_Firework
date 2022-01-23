// Result Shading Fragment Shader (Texture Mix)
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D screen;
uniform sampler2D bloom;

void main()
{
	FragColor = texture(screen,TexCoord) + texture(bloom,TexCoord);
}
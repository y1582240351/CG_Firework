#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform float intensity;
const int lights_max_num = 100;
uniform vec3 lights_position[lights_max_num];
uniform vec3 lights_color[lights_max_num];
uniform int lights_num;

void main()
{
    vec3 color = texture(texture_diffuse1, vs_out.TexCoords).xyz;
    vec3 viewDir = normalize(viewPos - vs_out.FragPos);
    vec3 normal = normalize(vs_out.Normal);

    vec3 lighting = vec3(0.0);
    for(int i = 0; i < lights_num; i++)
    {
        // diffuse
        vec3 lightDir = normalize(lights_position[i] - vs_out.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);

        // specular
        vec3 halfwayDir = normalize(viewDir + lightDir);
        float spec = pow(max(dot(halfwayDir, normal), 0.0), 64);

        // attenuation 
        float distance = length(vs_out.FragPos - lights_position[i]);

        lighting += (lights_color[i] * diff * color + vec3(spec)) * 1.0 * intensity / (distance * distance);
    }
    vec3 lighting_result = lighting + color * 0.2;

    // 模型不会发光，因此不需要使用高斯模糊实现泛光
    float brightness = dot(lighting_result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(lighting_result, 1.0);
    else
        BrightColor = vec4(0.1, 0.1, 0.1, 1.0);
    // BrightColor = vec4(0.13, 0.13, 0.13, 1.0);
    FragColor = vec4(lighting_result, 1.0);
}
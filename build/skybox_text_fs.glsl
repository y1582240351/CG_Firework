#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 Normal;
in vec3 FragPos;

uniform samplerCube skybox;
const int lights_max_num = 100;
uniform vec3 lights_position[lights_max_num];
uniform vec3 lights_color[lights_max_num];
uniform int lights_num;
uniform vec3 viewPos;
uniform float intensity;

void main()
{    
    vec3 color = texture(skybox, FragPos).rgb;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 normal = normalize(Normal);

    vec3 lighting = vec3(0.0);
    for(int i = 0; i < lights_num; i++)
    {
        // diffuse
        vec3 lightDir = normalize(lights_position[i] - FragPos);
        float diff = max(dot(lightDir, normal), 0.0);

        // specular
        vec3 halfwayDir = normalize(viewDir + lightDir);
        float spec = pow(max(dot(halfwayDir, normal), 0.0), 64);

        // attenuation 
        float distance = length(FragPos - lights_position[i]);

        // lighting += (lights_color[i] * diff * color + vec3(spec)) * 1.0 * intensity / (distance * distance);
        lighting += (lights_color[i] * diff * color) * 1.0 * intensity / (distance * distance);
    }
    vec3 lighting_result = lighting + color * 0.2;

    // »Ô¹âÐ§¹û
    float brightness = dot(lighting_result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(lighting_result, 1.0);
    else
        BrightColor = vec4(0.1, 0.1, 0.1, 1.0);
    FragColor = vec4(lighting_result, 1.0);
}
//#version 330 core
//layout (location=0) out vec4 FragColor;
//layout (location=1) out vec4 BrightColor;
//
//in vec3 TexCoords;
//
//uniform samplerCube skybox;
//
//void main()
//{    
//    FragColor = texture(skybox, TexCoords);
//    float brightness = dot(FragColor.rgb,vec3(0.2126,0.7152,0.0722));
//    if(brightness > 0.1)
//        BrightColor = FragColor;
//    else
//        BrightColor = vec4(0.1, 0.1, 0.1, 1.0);;
//}
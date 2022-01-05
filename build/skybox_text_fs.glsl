//#version 330 core
//out vec4 FragColor;
//
//in vec3 Normal;
//in vec3 FragPos;
//
//struct Light {
//    vec3 Position;
//    vec3 Color;
//    float Intensity;
//};
//
//uniform samplerCube skybox;
//uniform vec3 viewPos;
//uniform vec3 lightPos;
//
//void main()
//{    
//    vec3 color = texture(skybox, FragPos).rgb;
//    // ambient
//    vec3 ambient = 0.05 * color;
//    // diffuse
//    vec3 lightDir = normalize(lightPos - FragPos);
//    vec3 normal = normalize(Normal);
//    float diff = max(dot(lightDir, normal), 0.0);
//    vec3 diffuse = diff * color;
//    // specular
//    vec3 viewDir = normalize(viewPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = 0.0;
//    vec3 halfwayDir = normalize(lightDir + viewDir);  
//    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
//    vec3 specular = vec3(0.3) * spec;
//    // fragcolor
//    FragColor = vec4(ambient + diffuse + specular, 1.0);
//}
#version 330 core
layout (location=0) out vec4 FragColor;
layout (location=1) out vec4 BrightColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
    float brightness = dot(FragColor.rgb,vec3(0.2126,0.7152,0.0722));
    if(brightness > 0.1)
        BrightColor = FragColor;
    else
        BrightColor = vec4(0.1, 0.1, 0.1, 1.0);;
}
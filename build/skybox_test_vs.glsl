//#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
//
//out vec3 FragPos;
//out vec3 Normal;
//
//uniform mat4 projection;
//uniform mat4 view;
//
//void main()
//{
//    FragPos = aPos;
//    Normal = aNormal;
//    vec4 pos = projection * view * vec4(FragPos, 1.0);
//    gl_Position = pos.xyww;
//}  
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  
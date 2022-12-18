#version 330 core
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_tc;

out vec3 FragPos;
out vec3 Normal;
out vec2 tc;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    FragPos = vec3(modelMatrix * vec4(v_position, 1.0));
    Normal = mat3(transpose(inverse(modelMatrix))) * v_normal;  
    tc = v_tc;
    
    gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);
}
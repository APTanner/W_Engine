#version 330 core
layout (location = 0) in vec3 aPos;   
layout (location = 1) in vec3 aNormal; 
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aColor;

out vec3 Color;
out vec2 UV;

uniform mat4 localToWorld;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * localToWorld * vec4(aPos, 1.0);
    Color = aColor;
    UV = aUV;
} 

#version 330 core
out vec4 FragColor;  

in vec3 Color;
in vec2 UV;

uniform sampler2D t_diffuse;
uniform sampler2D t_specular;
uniform sampler2D t_normal;
  
void main()
{
    FragColor = vec4(Color, 1.0f);
}
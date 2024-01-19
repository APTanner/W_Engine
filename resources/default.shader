#version 330 core
layout (location = 0) in vec3 v_pos;   
layout (location = 1) in vec3 v_normal; 
layout (location = 2) in vec2 v_uv;
layout (location = 3) in vec3 v_color;

out vec3 f_color;
out vec2 f_uv;

uniform mat4 localToWorld;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * localToWorld * vec4(v_pos, 1.0);
    f_color = v_color;
    f_uv = v_uv;
} 

#version 330 core
out vec4 FragColor;  

in vec3 f_color;
in vec2 f_uv;

uniform sampler2D t_diffuse;
uniform sampler2D t_specular;
uniform sampler2D t_normal;

uniform vec3 m_diffuse;
uniform vec3 m_specular;
uniform vec3 m_ambient;
  
void main()
{
    vec4 texColor = texture(t_diffuse, f_uv);
    vec4 color = texColor * vec4(f_color, 1.0f);
    FragColor = texColor * vec4(m_diffuse, 1.0f);
}
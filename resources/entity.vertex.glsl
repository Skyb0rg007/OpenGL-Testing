#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_uv;
layout (location = 2) in vec3 normal;

out vec2 pass_texture_uv;
out vec3 frag_pos;
out vec3 pass_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    pass_texture_uv = texture_uv;
    pass_normal = mat3(transpose(inverse(model))) * normal;
    frag_pos = vec3(model * vec4(position, 1.0f));
}

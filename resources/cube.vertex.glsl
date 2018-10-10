#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coord;

out vec2 pass_texture_coord;

uniform mat4 modificationMatrix;

void main(void)
{
    gl_Position = modificationMatrix * vec4(position, 1.0f);
    pass_texture_coord = texture_coord;
}

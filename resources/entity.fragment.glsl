#version 330 core

in vec2 pass_texture_uv;
in vec3 pass_normal;
in vec3 frag_pos;

out vec4 out_color;

uniform sampler2D texture_sampler;
uniform vec3 light_pos;
uniform vec3 light_color;

void main(void)
{
    vec3 pixel_color = vec3(1.0f, 0.8f, 0.8f);// texture(texture_sampler, pass_texture_uv);

    /* ambient */
    float ambient_strength = 0.13f;
    vec3 ambient = ambient_strength * light_color;

    /* diffuse */
    vec3 norm = normalize(pass_normal);
    vec3 light_direction = normalize(light_pos - frag_pos);
    float diff = dot(norm, light_direction);
    diff = max(diff, 0.0f);
    vec3 diffuse = diff * light_color;

    /* specular */
    float specular_strength = 0.5f;
    // vec3 view_direction = normalize(view_pos)

    vec3 result = (diffuse + ambient) * pixel_color;
    out_color = vec4(result, 1.0f);
}

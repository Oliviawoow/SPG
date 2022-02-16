#version 430

// Input
layout(location = 0) in vec3 world_position_out;
layout(location = 1) in vec3 world_normal_out;

// Uniform properties
uniform sampler2D texture_1;
uniform samplerCube texture_cubemap;
uniform vec3 camera_position;
uniform float id;
uniform int on_greyscale;
uniform float piesa_apasata;

// Output
layout(location = 0) out vec4 out_color;

vec3 myReflect()
{
    vec3 razaIncidenta = world_position_out - camera_position;
    vec3 reflection = reflect(normalize(razaIncidenta), normalize(world_normal_out));
    return texture(texture_cubemap, reflection).xyz;
}

void main()
{
    vec3 mixt = mix(vec3(1, 1, 1), vec3(myReflect()), 0.45);
    out_color = vec4(mixt, id / 255.0f);

    if (on_greyscale == 1) {
        out_color = vec4(id / 255.0f, id / 255.0f, id / 255.0f, id / 255.0f);
    }
}

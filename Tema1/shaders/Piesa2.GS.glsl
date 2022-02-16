#version 430

layout(lines) in;
layout(triangle_strip, max_vertices = 256) out;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p01, control_p110, control_p21, control_p31;
uniform vec3 control_p41, control_p51, control_p61, control_p71;
uniform vec3 control_p81, control_p91, control_p101, control_p111;
uniform int no_of_instances;
uniform int no_of_generated_points;

uniform float max_rotate;

// Input
in int instance[2];
in vec3 world_position[];
in vec3 world_normal[];

// Output
out vec3 world_position_out;
out vec3 world_normal_out;

vec3 rotateY(vec3 point, float u)
{
    float x = point.x * cos(u) - point.z *sin(u);
    float z = point.x * sin(u) + point.z *cos(u);
    return vec3(x, point.y, z);
}

vec3 bezier1(float t)
{
    return  control_p01 * pow((1 - t), 3) +
            control_p110 * 3 * t * pow((1 - t), 2) +
            control_p21 * 3 * pow(t, 2) * (1 - t) +
            control_p31 * pow(t, 3);
}

vec3 bezier2(float t)
{
    return  control_p41 * pow((1 - t), 3) +
            control_p51 * 3 * t * pow((1 - t), 2) +
            control_p61 * 3 * pow(t, 2) * (1 - t) +
            control_p71 * pow(t, 3);
}

vec3 bezier3(float t)
{
    return  control_p81 * pow((1 - t), 3) +
            control_p91 * 3 * t * pow((1 - t), 2) +
            control_p101 * 3 * pow(t, 2) * (1 - t) +
            control_p111 * pow(t, 3);
}

void main()
{
    if (instance[0] < no_of_instances)
    {
        float deltaT = 1.0 / no_of_generated_points;
        float deltaRotate = 1.0 / max_rotate;
        float t;

        for (t = 0.0; t <= 1.0; t += deltaT) {
			vec3 bezier_point_crt_instance = rotateY(bezier1(t), instance[0] * deltaRotate);
            world_position_out = vec3(Model * vec4(bezier_point_crt_instance, 1));
	        world_normal_out = bezier1((t + 1) / no_of_generated_points);
            gl_Position = Projection * View * Model * vec4(bezier_point_crt_instance, 1);
            EmitVertex();

			vec3 bezier_point_next_instance = rotateY(bezier1(t), (instance[0] + 1) * deltaRotate);
	        world_position_out = vec3(Model * vec4(bezier_point_next_instance, 1));
	        world_normal_out = bezier1((t + 1) / no_of_generated_points);
            gl_Position = Projection * View * Model * vec4(bezier_point_next_instance, 1);
            EmitVertex();
		}
		EndPrimitive();

		for (t = 0.0; t <= 1.0; t += deltaT) {
			vec3 bezier_point_crt_instance = rotateY(bezier2(t), instance[0] * deltaRotate);
            world_position_out = vec3(Model * vec4(bezier_point_crt_instance, 1));
	        world_normal_out = bezier2((t + 1) / no_of_generated_points);
            gl_Position = Projection * View * Model * vec4(bezier_point_crt_instance, 1);
            EmitVertex();

			vec3 bezier_point_next_instance = rotateY(bezier2(t), (instance[0] + 1) * deltaRotate);
            world_position_out = vec3(Model * vec4(bezier_point_next_instance, 1));
            world_normal_out = bezier2((t + 1) / no_of_generated_points);
            gl_Position = Projection * View * Model * vec4(bezier_point_next_instance, 1);
            EmitVertex();
		}
		EndPrimitive();

		for (t = 0.0; t <= 1.0; t += deltaT) {
			vec3 bezier_point_crt_instance = rotateY(bezier3(t), instance[0] * deltaRotate);
            world_position_out = vec3(Model * vec4(bezier_point_crt_instance, 1));
	        world_normal_out = bezier3((t + 1) / no_of_generated_points);
            gl_Position = Projection * View * Model * vec4(bezier_point_crt_instance, 1);
            EmitVertex();

			vec3 bezier_point_next_instance = rotateY(bezier3(t), (instance[0] + 1) * deltaRotate);
            world_position_out = vec3(Model * vec4(bezier_point_next_instance, 1));
            world_normal_out = bezier3((t + 1) / no_of_generated_points);
            gl_Position = Projection * View * Model * vec4(bezier_point_next_instance, 1);
            EmitVertex();
		}
        EndPrimitive();
    }
}

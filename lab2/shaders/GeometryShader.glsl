#version 430

// Input and output topologies
// TODO(student): First, generate a curve (via line strip),
// then a rotation/translation surface (via triangle strip)
layout(lines) in;
layout(line_strip, max_vertices = 256) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p0, control_p1, control_p2, control_p3;
uniform int no_of_instances;
// TODO(student): Declare any other uniforms here
uniform int no_of_generated_points;
uniform float max_translate;
uniform float max_rotate;

// Input
in int instance[2];


vec3 rotateY(vec3 point, float u)
{
    float x = point.x * cos(u) - point.z *sin(u);
    float z = point.x * sin(u) + point.z *cos(u);
    return vec3(x, point.y, z);
}


vec3 translateX(vec3 point, float t)
{
    return vec3(point.x + t, point.y, point.z);
}


// This function computes B(t) with 4 control points. For a visual example,
// see [1]. For an interactive Javascript example with the exact points in
// this code, see [2].
//
// [1] https://www.desmos.com/calculator/cahqdxeshd
// [2] https://jsfiddle.net/6yuv9htf/
vec3 bezier(float t)
{
    return  control_p0 * pow((1 - t), 3) +
            control_p1 * 3 * t * pow((1 - t), 2) +
            control_p2 * 3 * pow(t, 2) * (1 - t) +
            control_p3 * pow(t, 3);
}


// TODO(student): If you want to take things a step further, try drawing a
// Hermite spline. Hint: you can repurpose two of the control points. For a
// visual example, see [1]. For an interactive Javascript example with the
// exact points in this code, see [2].
// 
// Unlike the Javascript function, you MUST NOT call the Bezier function.
// There is another way to draw a Hermite spline, all you need is to find
// the formula.
//
// [1] https://www.desmos.com/calculator/5knm5tkr8m
// [2] https://jsfiddle.net/6yuv9htf/

vec3 hermite(float t, vec3 Tp0, vec3 Tp3)
{
    return (2 * t * t * t - 3 * t * t + 1) * control_p0 +
    (-2 * t * t * t + 3 * t * t) * control_p3 +
    (t * t * t - 2 * t * t + t) * Tp0 + (t * t * t - t * t) * Tp3;
}

void main()
{
    const int SURFACE_TYPE_ROTATION     = 0;
    const int SURFACE_TYPE_TRANSLATION  = 1;

    // You can change the value of SURFACE_TYPE to experiment
    // with different transformation types.
    const int SURFACE_TYPE = SURFACE_TYPE_ROTATION;

    vec3 Tp0 = control_p1 - control_p0; // tangenta in punctul initial
    vec3 Tp3 = control_p2 - control_p3; // tangenta in punctul final

    if (instance[0] < no_of_instances)
    {
        // TODO(student): Rather than emitting vertices for the control
        // points, you must emit vertices that approximate the curve itself.
        //gl_Position = Projection * View * vec4(control_p0, 1);   EmitVertex();
        //gl_Position = Projection * View * vec4(control_p1, 1);   EmitVertex();
        //gl_Position = Projection * View * vec4(control_p2, 1);   EmitVertex();
        //gl_Position = Projection * View * vec4(control_p3, 1);   EmitVertex();
        float deltaT = 1.0 / no_of_generated_points;
        float deltaTranslate = 1.0 / max_translate;
        float deltaRotate = 1.0 / max_rotate;
        float t;

        for (t = 0.0; t <= 1.0; t += deltaT) {
            // ex2
            //vec3 bezier_point = bezier(t);
            //gl_Position = Projection * View * vec4(bezier_point, 1);
            //EmitVertex();

            // ex3 + 4
            //vec3 bezier_point_crt_instance = translateX(bezier(t), instance[0] * deltaTranslate);
            // or
            vec3 bezier_point_crt_instance = rotateY(bezier(t), instance[0] * deltaRotate);

            gl_Position = Projection * View * vec4(bezier_point_crt_instance, 1);
            EmitVertex();

            //vec3 bezier_point_next_instance = translateX(bezier(t), (instance[0] + 1) * deltaTranslate);
            // or
            vec3 bezier_point_next_instance = rotateY(bezier(t), (instance[0] + 1) * deltaRotate);

            gl_Position = Projection * View * vec4(bezier_point_next_instance, 1);
            EmitVertex();

            // bonus Hermite
            //vec3 hermite_point = hermite(t, Tp0, Tp3);
            //gl_Position = Projection * View * vec4(hermite_point, 1);
            //EmitVertex();

        }
        EndPrimitive();
    }
}
#version 450

layout(location = 0) in vec3 o_Color;

layout(location = 0) out vec4 o_FragColor;

void main()
{
    vec3 color = pow(o_Color, vec3(1.0 / 2.2));
    o_FragColor = vec4(color, 1.0);
}
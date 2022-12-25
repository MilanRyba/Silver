#version 450

layout(location = 0) in vec3 o_Color;

layout(location = 0) out vec4 o_FragColor;

void main()
{
    o_FragColor = vec4(o_Color, 1.0);
}
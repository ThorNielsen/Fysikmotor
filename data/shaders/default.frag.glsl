#version 330 core

in vec3 nml;
out vec4 colour;


uniform vec4 object_colour;
uniform vec3 ldir0;
uniform float lint0;
uniform vec3 ldir1;
uniform float lint1;
uniform vec3 ldir2;
uniform float lint2;

void main()
{
    float k = max(0.0f, dot(ldir0, nml)) * lint0;
    k += max(0.0f, dot(ldir1, nml)) * lint1;
    k += max(0.0f, dot(ldir2, nml)) * lint2;
    colour = k * object_colour;
}

#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 model_camera;
uniform mat4 model_world;

out vec3 nml;

void main()
{
    gl_Position = model_camera * position;
    //nml = vec3(abs(position.x), abs(position.y), 0.0f);//abs(position.z));
    nml = (vec4(normal, 0.0f) * model_world).xyz;
}

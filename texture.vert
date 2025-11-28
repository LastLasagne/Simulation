#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

layout(location = 0) out vec3 vColor;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec3 vFragPos;

layout(push_constant) uniform mod {
    mat4 model;
} model;

layout(set = 0, binding = 0) uniform cam {
    mat4 view;
    mat4 projection;
    vec3 lightPos;
    vec3 viewPos;
} camera;

out gl_PerVertex { vec4 gl_Position; };

void main()
{
    vColor = color;
    vNormal = normal;
    vFragPos = position;
    gl_Position = camera.projection * camera.view * model.model * vec4(position, 1.0);
}


#version 450

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vFragPos;
layout(location = 3) in vec3 camPos;
layout(location = 4) in vec3 lightPos;

layout(location = 0) out vec4 fragColor;

layout(set = 1, binding = 0) uniform sampler2D textureSampler;

const vec3 lightColor = vec3(1.0, 1.0, 1.0);

const vec3 ambientColor = vec3(0.2);
const float shininess   = 32.0;

void main()
{
    vec3 N = normalize(vNormal);

    vec3 L = normalize(lightPos - vFragPos);

    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 V = normalize(camPos - vFragPos);
    vec3 R = reflect(-L, N);                          // light reflection
    float spec = pow(max(dot(V, R), 0.0), shininess);
    vec3 specular = spec * lightColor * 0.5;          // reduce specular strength

    vec3 color = (ambientColor + diffuse + specular) * vColor;

    fragColor = vec4(color, 1.0);
}

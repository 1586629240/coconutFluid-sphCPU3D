#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 trans;

out vec4 ballAttr;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * vec4(0.2*(aPos+trans.xyz), 1.0f);
    ballAttr = trans;
}
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 ship;

void main()
{
    gl_Position = projection *  view * ship * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
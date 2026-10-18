#version 460 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in float a_TexIndex;

out vec2 o_TexCords;
flat out float o_TexIndex;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
	gl_Position = u_ViewProjectionMatrix * vec4(a_Pos, 1.0);
	o_TexCords = a_TexCoords;
	o_TexIndex = a_TexIndex;
}
#version 430 core
layout (location = 0) in vec4 position;
layout (location = 3) in vec4 trans;

uniform mat4 view;
uniform mat4 projection;

out vec3 eyeSpacePos;

void main(){
	//position.xyz=0.2*(position.xyz+trans.xyz);
	eyeSpacePos = (view * vec4(0.2*(5*position.xyz+trans.xyz), 1.0f)).xyz;
	gl_Position = projection * view * vec4(0.2*(5*position.xyz+trans.xyz), 1.0f);
}
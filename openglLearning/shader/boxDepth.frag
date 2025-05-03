#version 430 core

uniform mat4 projection;

in vec3 eyeSpacePos;

float near=0.1;
float far=1000;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // ×ª»»Îª NDC
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main(){	
	gl_FragDepth = LinearizeDepth(gl_FragCoord.z);
}
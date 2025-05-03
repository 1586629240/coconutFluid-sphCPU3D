#version 430 core

uniform float pointSize;
uniform mat4 projectMatrix;

in vec3 eyeSpacePos;
layout(location = 0) out vec4 fragColor;

void main(){	
	vec3 normal;
	normal.xy = gl_PointCoord.xy * vec2(2.0, -2.0) + vec2(-1.0,1.0);
	float mag = dot(normal.xy, normal.xy)*0.2;
	if(mag > 1) discard;
	normal.z = sqrt(1 - mag);

	vec4 pixelEyePos = vec4(eyeSpacePos + normal * pointSize, 1.0f);
	vec4 pixelClipPos = projectMatrix * pixelEyePos;
	float ndcZ = pixelClipPos.z / pixelClipPos.w;
	gl_FragDepth = ndcZ;

	fragColor = vec4(normal.z*0.005,0,0, 1.0);
}
#version 330 core
out vec4 FragColor;

in vec4 ballAttr;//{x,y,z,hue}

uniform vec4 lightAttr; //{x,y,z,hue}

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

vec3 hsv2rgb(vec3 c) {
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    FragColor=vec4(hsv2rgb(vec3(ballAttr.w,1,1)),1);
}
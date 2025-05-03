#version 330 core
out vec4 FragColor;
in vec4 position;
in float color;

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

void main()
{
    float gray = lerp(0.40,0.15,position.x)+0.2;
    FragColor = gray*vec4(vec3(color,color,color),1);
}
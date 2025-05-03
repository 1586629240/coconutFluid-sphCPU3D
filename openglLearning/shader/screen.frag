#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool isDepth;

void main() {
    if(isDepth) {
        // 读取深度值并显示为灰度
        float depthValue = texture(screenTexture, TexCoords).r;
        FragColor = vec4(vec3(depthValue), 1.0);
    } else {
        // 直接显示颜色缓冲
        FragColor = texture(screenTexture, TexCoords);
    }
}
#pragma once
#include <glad/glad.h>
#include "../gl/shader.h"
#include "./ScreenRender.h"
#include "../gl/OffScreenBuffer.h"
#include "../gl/Camera.h"

class LiquidRender
{
    ScreenRender mesh;
    OffScreenBuffer screenBuffer;
    Shader liquidShader, blurShader;

    void blurTexture(GLuint tex, glm::mat4 projectMat);

public:
    LiquidRender();
    void draw(Camera& c, OffScreenBuffer& sphBuffer, OffScreenBuffer& bgBuffer);
    GLuint GetColorTexture();
};

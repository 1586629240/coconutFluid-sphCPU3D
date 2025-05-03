#pragma once
#include "../gl/shader.h"

class ScreenRender
{
public:
    ScreenRender();

    ~ScreenRender();

    void draw(GLuint textureID, bool isDepth);
    void drawMesh();
private:
    Shader shader;
    GLuint VAO, VBO;
};

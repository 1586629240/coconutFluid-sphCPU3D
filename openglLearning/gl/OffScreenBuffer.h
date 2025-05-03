#pragma once
#include <glad/glad.h>
class OffScreenBuffer
{
public:
    OffScreenBuffer(unsigned int width, unsigned int height);

    ~OffScreenBuffer();

    void bind();

    void unbind();

    GLuint GetColorTexture() const;
    GLuint GetDepthTexture() const;
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

private:
    GLuint fbo = 0;
    GLuint colorTex = 0;
    GLuint depthTex = 0;
    unsigned int width;
    unsigned int height;

    void InitializeFBO();

    void ConfigureTextureParameters();

    void ConfigureDepthTextureParameters();
};
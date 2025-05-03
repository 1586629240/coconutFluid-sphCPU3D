#include "ScreenRender.h"

ScreenRender::ScreenRender() :shader("./shader/screen.vert", "./shader/screen.frag")
{
    float vertices[] = {
        -1.0f,  1.0f, 0.0f, 1.0f, // 左上
        -1.0f, -1.0f, 0.0f, 0.0f, // 左下
        1.0f,  1.0f, 1.0f, 1.0f, // 右上
        1.0f, -1.0f, 1.0f, 0.0f  // 右下
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

ScreenRender::~ScreenRender()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void ScreenRender::draw(GLuint textureID, bool isDepth)
{
    shader.use();
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    shader.setInt("screenTexture", 0);
    shader.setBool("isDepth", isDepth);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    shader.unuse();
}

void ScreenRender::drawMesh()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

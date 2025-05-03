#include "LiquidRender.h"

void LiquidRender::blurTexture(GLuint tex, glm::mat4 projectMat)
{
    screenBuffer.bind();
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    blurShader.use();
    blurShader.setInt("image", 0);
    blurShader.setFloat("filterRadius", 5);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    for (unsigned int index = 0; index < 5; ++index)
    {
        mesh.drawMesh();
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 1920, 1080);
    }

    screenBuffer.unbind();
}

LiquidRender::LiquidRender() :
    liquidShader("./shader/liquid.vert", "./shader/liquid.frag"),
    blurShader("./shader/depthBlur.vert", "./shader/depthBlur.frag"),
    screenBuffer(1920, 1080)
{

}

void LiquidRender::draw(Camera& c, OffScreenBuffer& sphBuffer, OffScreenBuffer& bgBuffer)
{
    blurTexture(sphBuffer.GetDepthTexture(), c.getProjectionMatrix());

    screenBuffer.bind();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    liquidShader.use();

    liquidShader.setInt("depthTex", 0);
    liquidShader.setInt("thicknessTex", 1);
    liquidShader.setInt("backgroundTex", 2);
    liquidShader.setInt("backgroundDepthTex", 3);
    liquidShader.setVec4("liquidColor", glm::vec4(.22, .59, .90, 1.));
    liquidShader.setMat4f("viewMatrix", c.GetViewMatrix());
    liquidShader.setMat4f("invViewMatrix", c.getInvViewMatrix());
    liquidShader.setMat4f("invProjectMatrix", c.getInvProjectMatrix());
    liquidShader.setVec3("dirLight.direction", glm::vec3(0, 1, 0));
    liquidShader.setVec3("dirLight.diffuse", glm::vec3(1, 1, 1));
    liquidShader.setVec3("dirLight.ambient", glm::vec3(1, 1, 1));
    liquidShader.setVec3("dirLight.specular", glm::vec3(1, 1, 1));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sphBuffer.GetDepthTexture());
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, sphBuffer.GetColorTexture());
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, bgBuffer.GetColorTexture());
    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, bgBuffer.GetDepthTexture());

    mesh.drawMesh();

    screenBuffer.unbind();
}

GLuint LiquidRender::GetColorTexture()
{
    return screenBuffer.GetColorTexture();
}

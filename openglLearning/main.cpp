#include "gl/window.h"
#include "glRender/sphRender.h"
#include "glRender/boxRender.h"
#include "glRender/worldRender.h"

int main()
{
    GLFWwindow* window = initGL();
    if (window == nullptr)return -1;

    Eigen::Vector4f lightAttr = { 62.5f,100.f,62.5f,1.f };

    LiquidRender liquidRender;
    ScreenRender screenRender;

    auto& world = WorldRender::getInstance();
    auto& sphRender = sphGLRender::getInstance();
    auto& sphSys = sphRender.getSPH();

    sphSys.addForce("gravity", std::make_shared<gravityField>());
    world.addObject("lightSrc0", std::make_shared<boxRender>(lightAttr));

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
		sphRender.updateSPH();

        world.renderdOffScreen(camera);
        sphRender.renderdFluid(camera);
        liquidRender.draw(camera, sphRender.getScreenBuffer(), world.getScreenBuffer());
        screenRender.draw(liquidRender.GetColorTexture(), 0);
		world.renderdOnScreen(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
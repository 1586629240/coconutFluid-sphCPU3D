#pragma once
#include "glRenderBase.h"
#include "../gl/OffScreenBuffer.h"
#include <map>
#include <string>
#include <memory>
class WorldRender
{
    OffScreenBuffer screenFrame;
    std::map<std::string, std::shared_ptr<glRenderBase>> worldObjs;
public:
    WorldRender() : screenFrame(1920, 1080) {}
    void addObject(const char* name, std::shared_ptr<glRenderBase> renderPtr)
    {
        worldObjs[name] = renderPtr;
    }
    std::shared_ptr<glRenderBase> getObject(const char* name)
    {
        return worldObjs[name];
    }
    void renderdOffScreen(Camera& c)
    {
        screenFrame.bind();
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        //ÑÕÉ«»º³å
        for (auto& x : worldObjs)x.second->renderdColorMap(c);
        //Éî¶È»º³å
        for (auto& x : worldObjs)x.second->renderdDepthMap(c);
        screenFrame.unbind();
    }
    void renderdOnScreen(Camera& c)
    {
        for (auto& x : worldObjs)x.second->renderdColorMap(c);
    }
    OffScreenBuffer& getScreenBuffer() { return screenFrame; }
    GLuint GetColorTexture() { return screenFrame.GetColorTexture(); }
    GLuint GetDepthTexture() { return screenFrame.GetDepthTexture(); }

    static WorldRender& getInstance()
    {
        static WorldRender instance;
        return instance;
    }
    WorldRender& operator=(const WorldRender&) = delete;
    WorldRender(const WorldRender&) = delete;
};
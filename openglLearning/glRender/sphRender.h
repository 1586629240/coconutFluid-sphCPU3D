#pragma once
#include "../SPH/SPHsystem.h"
#include "glRenderBase.h"

#include "./LiquidRender.h"
class sphGLRender :public glRenderBase
{
	struct attrPack {
		Eigen::Vector4f pos;
	};

	SPHsystem& sph;
	Eigen::Vector4f lightAttr; //{x,y,z,hue}
	std::shared_ptr<attrPack[]> ballAttr;//{pos,norm} pos{x,y,z,hue} norm{x,y,z,0}

	unsigned ballCnt = 0, VBO;

    Shader fluidShader;
    ScreenRender screenRender;
    OffScreenBuffer fluidFrame;

    void drawSPHBalls();
	void updateVertGLData();

    void setColorCamUniforms(Camera& c);
    void setFluidCamUniforms(Camera& c);
public:
	sphGLRender();

    void renderdFluid(Camera& c);
	void renderdColorMap(Camera& c) override;

    void updateSPH();
	SPHsystem& getSPH();
    OffScreenBuffer& getScreenBuffer();

	static sphGLRender& getInstance();
	void setLightAttr(Eigen::Vector4f lightAttr);

	sphGLRender(const sphGLRender&) = delete;
	sphGLRender& operator=(const sphGLRender&) = delete;

	~sphGLRender();
};
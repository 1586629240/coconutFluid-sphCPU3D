#pragma once

#include <Eigen/core>
#include "glRenderBase.h"

class boxRender :public glRenderBase
{
	unsigned VBO;
	Shader depthShader;
	Eigen::Vector4f lightAttr;

public:
	boxRender(Eigen::Vector4f lightAttr);
	~boxRender() = default;

	void renderdColorMap(Camera& c) override;
	void renderdDepthMap(Camera& c) override;
	void setAttribute(Eigen::Vector4f pos);
};


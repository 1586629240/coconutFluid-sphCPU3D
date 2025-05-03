#pragma once
#include "../gl/model.h"
#include "../gl/shader.h"
#include "../gl/camera.h"

class glRenderBase
{
protected:
	Model obj;
	Shader shader;

public:
	glRenderBase(const char* obj,const char* vert,const char* frag):
		obj(obj), shader(vert, frag) {}

	virtual void renderdColorMap(Camera& c) {}
	virtual void renderdDepthMap(Camera& c) {}
	virtual ~glRenderBase() = 0 {}
};
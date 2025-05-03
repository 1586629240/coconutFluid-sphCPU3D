#include "sphRender.h"

void sphGLRender::updateVertGLData()
{
	size_t curIdx = 0;
	for (size_t z = 1; z <= sph.gridnz(); z++)
		for (size_t y = 1; y <= sph.gridny(); y++)
			for (size_t x = 1; x <= sph.gridnx(); x++)
			{
				if (sph.getPos(x, y, z).size() == 0)continue;
				auto pos = sph.getPos(x, y, z);
				auto vel = sph.getVel(x, y, z);
				for (size_t i = 0; i < pos.size(); i++) {
					ballAttr[curIdx].pos = Eigen::Vector4f(pos[i][0] / 4, pos[i][1] / 4, pos[i][2] / 4, vel[i].norm() / 50000);
					curIdx++;
				}
			}
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, ballCnt * sizeof(sphGLRender::attrPack), &ballAttr[0], GL_STATIC_DRAW);
}

void sphGLRender::drawSPHBalls()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	for (unsigned int i = 0; i < obj.meshes.size(); i++)
	{
		glBindVertexArray(obj.meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(obj.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, ballCnt);
	}
	glBindVertexArray(0);
}

void sphGLRender::setColorCamUniforms(Camera& c) //just for only draw balls
{
	shader.use();
	glm::mat4 projection = c.getProjectionMatrix();
	glm::mat4 view = c.GetViewMatrix();

	shader.setVec4("lightAttr", glm::make_vec4(lightAttr.data()));
	shader.setMat4f("projection", glm::value_ptr(projection));
	shader.setMat4f("view", glm::value_ptr(view));
}

void sphGLRender::setFluidCamUniforms(Camera& c)
{
	float densityLowerBound = 1.0f / (8.0f * pow(sph.arg().ballR, 3.0f)) * 0.001f;
	glm::mat4 projection = c.getProjectionMatrix();
	glm::mat4 view = c.GetViewMatrix();

	fluidShader.use();
	fluidShader.setFloat("farPlane", 0.1);
	fluidShader.setFloat("nearPlane", 1000);
	fluidShader.setFloat("pointScale", 0.2);
	fluidShader.setFloat("pointSize", 4.0f * sph.arg().ballR);
	fluidShader.setFloat("densityLowerBound", densityLowerBound);
	fluidShader.setMat4f("viewMatrix", glm::value_ptr(view));
	fluidShader.setMat4f("projectMatrix", glm::value_ptr(projection));
}

sphGLRender::sphGLRender() :
	glRenderBase("./object/ball.obj", "./shader/ball.vert", "./shader/ball.frag"),
	fluidShader("./shader/sphFluid.vert", "./shader/sphFluid.frag"),
	sph(SPHsystem::getInstance()), fluidFrame(1920, 1080)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	ballCnt = sph.curBallCnt();
	//sph.addForce("gravity", std::make_shared<gravityField>());
	sph.addCollider("wall", std::make_shared<BoundryCollider>(sph.arg()));
	for (unsigned int i = 0; i < obj.meshes.size(); i++)
	{
		glBindVertexArray(obj.meshes[i].VAO);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(sphGLRender::attrPack), (void*)0);
		glVertexAttribDivisor(3, 1);

		glBindVertexArray(0);
	}
	lightAttr = Eigen::Vector4f::Zero();
	ballAttr = std::make_shared<sphGLRender::attrPack[]>(ballCnt);
}

void sphGLRender::updateSPH()
{
	updateVertGLData();
	sph.update();
}

SPHsystem& sphGLRender::getSPH() { return sph; }

OffScreenBuffer& sphGLRender::getScreenBuffer()
{
	return fluidFrame;
}

sphGLRender& sphGLRender::getInstance()
{
	static sphGLRender instance;
	return instance;
}

void sphGLRender::setLightAttr(Eigen::Vector4f lightAttr)
{
	this->lightAttr = lightAttr;
}

sphGLRender::~sphGLRender()
{
	glDeleteBuffers(1, &VBO);
}

void sphGLRender::renderdFluid(Camera& c)
{
	updateSPH();
	fluidFrame.bind();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	fluidShader.use();
	setFluidCamUniforms(c);
	drawSPHBalls();
	glDisable(GL_PROGRAM_POINT_SIZE);

	fluidFrame.unbind();
}

void sphGLRender::renderdColorMap(Camera& c)
{
	glClearColor(0.1, 0.1, 0.1, 1.0);
	setColorCamUniforms(c);
	drawSPHBalls();
}
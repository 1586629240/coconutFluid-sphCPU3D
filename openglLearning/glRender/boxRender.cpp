#include "boxRender.h"

boxRender::boxRender(Eigen::Vector4f lightAttr) :
	glRenderBase("./object/box.obj", "./shader/box.vert", "./shader/box.frag"),
	depthShader("./shader/boxDepth.vert", "./shader/boxDepth.frag")
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	for (unsigned int i = 0; i < obj.meshes.size(); i++)
	{
		glBindVertexArray(obj.meshes[i].VAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Eigen::Vector4f), (void*)0);
		glVertexAttribDivisor(3, 1);
		glBindVertexArray(0);
	}
	setAttribute(lightAttr);
}

void boxRender::renderdColorMap(Camera& c)
{
	shader.use();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Eigen::Vector4f), &lightAttr, GL_STATIC_DRAW);

	glm::mat4 projection = glm::perspective(c.getFov(), (float)1920. / (float)1080., 0.1f, 1000.0f);
	glm::mat4 view = c.GetViewMatrix();
	shader.setMat4f("projection", glm::value_ptr(projection));
	shader.setMat4f("view", glm::value_ptr(view));

	for (unsigned int i = 0; i < obj.meshes.size(); i++)
	{
		glBindVertexArray(obj.meshes[i].VAO);
		glDrawArrays(GL_TRIANGLES, 0, obj.meshes[i].indices.size());
	}
	glBindVertexArray(0);
}

void boxRender::renderdDepthMap(Camera& c)
{
	glEnable(GL_DEPTH_TEST);

	depthShader.use();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Eigen::Vector4f), &lightAttr, GL_STATIC_DRAW);

	glm::mat4 projection = glm::perspective(c.getFov(), (float)1920. / (float)1080., 0.1f, 1000.0f);
	glm::mat4 view = c.GetViewMatrix();
	depthShader.setMat4f("projection", glm::value_ptr(projection));
	depthShader.setMat4f("view", glm::value_ptr(view));

	for (unsigned int i = 0; i < obj.meshes.size(); i++)
	{
		glBindVertexArray(obj.meshes[i].VAO);
		glDrawArrays(GL_TRIANGLES, 0, obj.meshes[i].indices.size());
	}
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);
}

void boxRender::setAttribute(Eigen::Vector4f lightAttr)
{
	this->lightAttr = lightAttr;
}

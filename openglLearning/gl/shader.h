#pragma once

#include <string>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader{
	std::string readFile(const char* fileName);
	void checkError(unsigned id, const char* name);
	void initShaderProgram(const char* verShaderCode, const char* fragShaderCode,const char* geomCode);
public:
	// ≥Ã–ÚID
	unsigned int ID;

	void use();
	void unuse();
	void setInt(const std::string& name, int value)     const;
	void setBool(const std::string& name, bool value)   const;
	void setVec3(const std::string& name, glm::vec3 v)  const;
	void setVec4(const std::string& name, glm::vec4 v)  const;
	void setFloat(const std::string& name, float value) const;
	void setMat4f(const std::string& name, float* value)const;
	void setMat4f(const std::string& name, glm::mat4 value) const;
	void setVec3(const std::string& name, float a, float b, float c)const;
	void setVec4(const std::string& name, float a, float b, float c, float d)const;

	~Shader();
	Shader(const char* vertexPath, const char* fragmentPath, const char* geom = nullptr);
};
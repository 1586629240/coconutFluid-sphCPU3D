#include "shader.h"

void Shader::checkError(unsigned id, const char* name) {
	static GLint success;
	static char buffer[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, buffer);
		printf("ERROR in:" "\033[0;31m" " %s" "\033[41;37;1m" "\n%s" "\033[0m", name, buffer);
	}
}
void Shader::initShaderProgram(const char* verShaderCode, const char* fragShaderCode, const char* geomCode) {
	unsigned int vertex, fragment, geom;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &verShaderCode, NULL);
	glCompileShader(vertex); checkError(vertex, "VERTEX_SHADER_COMPILE");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragShaderCode, NULL);
	glCompileShader(fragment); checkError(fragment, "FRAG_SHADER_COMPILE");

	ID = glCreateProgram();
	glAttachShader(ID, vertex), glAttachShader(ID, fragment);

	if (geomCode == nullptr)
	{
		glLinkProgram(ID), checkError(ID, "SHADE_PIPELINE_PROGRAOM");
		glDeleteShader(vertex), glDeleteShader(fragment);
	}
	else
	{
		geom = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom, 1, &geomCode, NULL);
		glCompileShader(geom); checkError(geom, "GEOM_SHADER_COMPILE");
		glAttachShader(ID, geom), glLinkProgram(ID), checkError(ID, "SHADE_PIPELINE_PROGRAOM");
		glDeleteShader(vertex), glDeleteShader(fragment), glDeleteShader(geom);
	}
}
std::string Shader::readFile(const char* fileName) {
	std::string content;
	std::ifstream file(fileName);
	for (std::string line; getline(file, line); content.append(line))
		line.append("\n");
	return content;
}
Shader::~Shader() { glDeleteProgram(ID); }
Shader::Shader(const char* vertex, const char* fragment, const char* geom) {
	auto vertexCode = readFile(vertex);
	auto fragmentCode = readFile(fragment);
	if (geom != nullptr) {
		auto gemoCode = readFile(geom);
		initShaderProgram(vertexCode.c_str(), fragmentCode.c_str(), gemoCode.c_str());
	}
	else
		initShaderProgram(vertexCode.c_str(), fragmentCode.c_str(), nullptr);
}
void Shader::use() { glUseProgram(ID); }
void Shader::unuse() { glUseProgram(0); }
void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setMat4f(const std::string& name, glm::mat4 value)const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4f(const std::string& name, float* value)const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}
void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec3(const std::string& name, float a, float b, float c)const
{
	glm::vec3 v{ a,b,c };
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v));
}
void Shader::setVec4(const std::string& name, float a, float b, float c, float d) const
{
	glm::vec4 v{ a,b,c,d };
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v));
}
void Shader::setVec3(const std::string& name, glm::vec3 v)const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v));
}

void Shader::setVec4(const std::string& name, glm::vec4 v) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v));
}
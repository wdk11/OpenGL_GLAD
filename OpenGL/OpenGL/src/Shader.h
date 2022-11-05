#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};
class Shader
{
public:
	unsigned int ID;//着色器项目的名字比如shaderprogram
	//读取数据并返回一个string
	ShaderProgramSource ParseShader(const std::string& filepath);
	//构建着色器
	Shader(const std::string& filepath);
	//使用激活程序
	void use();
	//uniform工具函数
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, float valuex, float valuey, float valuez) const;
	void setMat4(const std::string& name, glm::mat4 mat4) const;
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};
#endif
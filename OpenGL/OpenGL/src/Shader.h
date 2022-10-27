#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};
class Shader
{
public:
	unsigned int ID;//��ɫ����Ŀ�����ֱ���shaderprogram
	//��ȡ���ݲ�����һ��string
	ShaderProgramSource ParseShader(const std::string& filepath);
	//������ɫ��
	Shader(const std::string& filepath);
	//ʹ�ü������
	void use();
	//uniform���ߺ���
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};
#endif
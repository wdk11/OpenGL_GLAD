#ifndef SHADER_H
#define SHADER_H
#include <string>
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};
class Shader
{
public:
	unsigned int ID;//��ɫ����Ŀ�����ֱ���shaderprogram
};





#endif
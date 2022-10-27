#pragma once
#include "Shader.h"

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType//����ȡֵ�ģ������һ��
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];//����ַ��������������ֲ�ͬ����ɫ��
	ShaderType type = ShaderType::NONE;//��ʼ���õģ�����һ��ö��class
	while (getline(stream, line))//һ��һ�еĶ�ȡ����
	{
		if (line.find("#shader") != std::string::npos)//������һ�����棬�ҵ���shader�ٿ������
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;//��int(type)
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[int(type)] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };
}


Shader::Shader(const std::string& filepath)
{
	//��ȡ����
	ShaderProgramSource source = ParseShader(filepath);
	const char* vShaderCode = source.VertexSource.c_str();
	const char* fShaderCode = source.FragmentSource.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	std::cout << "VERTEX SHADER:" << std::endl << vShaderCode << std::endl << std::endl;
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	std::cout << "FRAGMENT SHADER:" << std::endl << fShaderCode << std::endl << std::endl;
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glValidateProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	////������ɫ���Ĵ���
	//unsigned int vertexShader;
	//vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//const char* VertexShaderSource = source.VertexSource.c_str();
	//glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);
	//glCompileShader(vertexShader);
	//checkCompileErrors(vertexShader, "VERTEX");
	////Ƭ����ɫ���Ĵ���
	//unsigned int fragmentShader;
	//fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//const char* fragmentShaderSource = source.VertexSource.c_str();
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//glCompileShader(fragmentShader);
	//checkCompileErrors(fragmentShader, "FRAGMENT");
	////��ɫ���Ĵ���
	//ID = glCreateProgram();
	//glAttachShader(ID, vertexShader);
	//glAttachShader(ID, fragmentShader);
	//glLinkProgram(ID);
	//checkCompileErrors(ID, "PROGRAM");
	////ɾ����ɫ��
	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

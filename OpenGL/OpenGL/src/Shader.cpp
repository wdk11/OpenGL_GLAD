#pragma once
#include "Shader.h"

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType//用来取值的，好理解一点
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];//这个字符串流来保存两种不同的着色器
	ShaderType type = ShaderType::NONE;//初始化用的，这是一个枚举class
	while (getline(stream, line))//一行一行的读取数据
	{
		if (line.find("#shader") != std::string::npos)//这是在一行里面，找到了shader再看后面的
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;//用int(type)
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
	//读取数据
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

	////顶点着色器的创建
	//unsigned int vertexShader;
	//vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//const char* VertexShaderSource = source.VertexSource.c_str();
	//glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);
	//glCompileShader(vertexShader);
	//checkCompileErrors(vertexShader, "VERTEX");
	////片段着色器的创建
	//unsigned int fragmentShader;
	//fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//const char* fragmentShaderSource = source.VertexSource.c_str();
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//glCompileShader(fragmentShader);
	//checkCompileErrors(fragmentShader, "FRAGMENT");
	////着色器的创建
	//ID = glCreateProgram();
	//glAttachShader(ID, vertexShader);
	//glAttachShader(ID, fragmentShader);
	//glLinkProgram(ID);
	//checkCompileErrors(ID, "PROGRAM");
	////删除着色器
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

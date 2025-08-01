#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader
{
public:
	// 程序ID
	unsigned int ID;

	//// 构造器读取并构建着色器
	//Shader(const char* vertexPath, const char* fragmentPath);
	//// 使用/激活程序
	//void use();
	//// uniform工具函数
	//void setBool(const string& name, bool value) const;
	//void setInt(const string& name, int value) const;
	//void setFloat(const string& name, float value) const;
public:
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. 从文件路径中获取顶点/片段着色器
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;
		// 保证ifstream对象可以抛出异常：
		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		try
		{
			// 打开文件
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;
			// 读取文件的缓冲内容到数据流中
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// 关闭文件处理器
			vShaderFile.close();
			fShaderFile.close();
			// 转换数据流到string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (ifstream::failure e)
		{
			cout << "读取着色器文件错误" << endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// 2. 编译着色器
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];
		// 顶点着色器
		vertex = glCreateShader(GL_VERTEX_SHADER);		//创建顶点着色器对象
		glShaderSource(vertex, 1, &vShaderCode, NULL);	//将着色器源码附加到顶点着色器对象
		glCompileShader(vertex);//编译顶点着色器
		// 打印编译错误（如果有的话）
		checkCompileErrors(vertex, "顶点");

		// 片段着色器
		fragment = glCreateShader(GL_FRAGMENT_SHADER);	//创建片段着色器对象
		glShaderSource(fragment, 1, &fShaderCode, NULL);//将着色器源码附加到片段着色器对象	
		glCompileShader(fragment);//编译片段着色器
		checkCompileErrors(fragment, "片段");

		// 着色器程序
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");// 打印连接错误（如果有的话）
		// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void use()
	{
		glUseProgram(ID);//激活使用着色器
	}
	void setBool(const string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec3(const string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setVec3(const string& name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
private:
	///<summary>检查编译失败日志</summary>
	void checkCompileErrors(unsigned int shader, string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (success)
			{
				cout << type << "着色器编译成功" << endl;
			}
			else
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				cout << type << "着色器编译失败 " << "\n" << infoLog << "\n -- ---------- -- " << endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (success)
			{
				cout << type << "着色器程序链接成功" << endl;
			}
			else
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				cout << "着色器程序链接失败 " << "\n" << infoLog << "\n -- ---------- -- " << endl;
			}
		}
	}
};

#endif
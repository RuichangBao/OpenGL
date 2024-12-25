#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // ����glad����ȡ���еı���OpenGLͷ�ļ�
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader
{
public:
	// ����ID
	unsigned int ID;

	//// ��������ȡ��������ɫ��
	//Shader(const char* vertexPath, const char* fragmentPath);
	//// ʹ��/�������
	//void use();
	//// uniform���ߺ���
	//void setBool(const string& name, bool value) const;
	//void setInt(const string& name, int value) const;
	//void setFloat(const string& name, float value) const;
public:
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;
		// ��֤ifstream��������׳��쳣��
		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		try
		{
			// ���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;
			// ��ȡ�ļ��Ļ������ݵ���������
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// �ر��ļ�������
			vShaderFile.close();
			fShaderFile.close();
			// ת����������string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (ifstream::failure e)
		{
			cout << "��ȡ��ɫ���ļ�����" << endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// 2. ������ɫ��
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];
		// ������ɫ��
		vertex = glCreateShader(GL_VERTEX_SHADER);		//����������ɫ������
		glShaderSource(vertex, 1, &vShaderCode, NULL);	//����ɫ��Դ�븽�ӵ�������ɫ������
		glCompileShader(vertex);//���붥����ɫ��
		// ��ӡ�����������еĻ���
		checkCompileErrors(vertex, "����");

		// Ƭ����ɫ��
		fragment = glCreateShader(GL_FRAGMENT_SHADER);	//����Ƭ����ɫ������
		glShaderSource(fragment, 1, &fShaderCode, NULL);//����ɫ��Դ�븽�ӵ�Ƭ����ɫ������	
		glCompileShader(fragment);//����Ƭ����ɫ��
		checkCompileErrors(fragment, "Ƭ��");

		// ��ɫ������
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");// ��ӡ���Ӵ�������еĻ���
		// ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void use()
	{
		glUseProgram(ID);//����ʹ����ɫ��
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
	///<summary>������ʧ����־</summary>
	void checkCompileErrors(unsigned int shader, string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (success)
			{
				cout << type << "��ɫ������ɹ�" << endl;
			}
			else
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				cout << type << "��ɫ������ʧ�� " << "\n" << infoLog << "\n -- ---------- -- " << endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (success)
			{
				cout << type << "��ɫ���������ӳɹ�" << endl;
			}
			else
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				cout << "��ɫ����������ʧ�� " << "\n" << infoLog << "\n -- ---------- -- " << endl;
			}
		}
	}
};

#endif
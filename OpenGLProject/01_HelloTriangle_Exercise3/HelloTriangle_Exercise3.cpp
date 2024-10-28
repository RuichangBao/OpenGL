//https://github.com/LearnOpenGL-CN/LearnOpenGL-CN/blob/new-theme/docs/01%20Getting%20started/04%20Hello%20Triangle.md
#include "HelloTriangle_Exercise3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	//glfw: ��ʼ������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//OpenGL�������ļ������� ����ģʽ
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//�����޸Ĵ��ڴ�С
#endif
	// glfw �������ڶ���
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HelloTriangle_Exercise3", NULL, NULL);
	if (window == NULL)
	{
		cout << "����GLFW����ʧ�ܣ�" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//ʹָ�����ڵ�������Ϊ�����̵߳ĵ�ǰ�����ġ�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//���ô��ڴ�С�仯�Ļص�����

	// glad: ��������OpenGL����ָ��
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "��ʼ��GLAD(OpenGL����ָ�����)ʧ��" << endl;
		return -1;
	}
	int success;
	char infoLog[512];

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);//����������ɫ������
	unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER); //������ɫƬ����ɫ������
	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER); //������ɫƬ����ɫ������
	unsigned int shaderProgramOrange = glCreateProgram();	//������ɫ��ɫ���������
	unsigned int shaderProgramYellow = glCreateProgram();	//������ɫ��ɫ���������

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//����ɫ��Դ�븳ֵ����ɫ��
	glCompileShader(vertexShader);	//���붥����ɫ��
	checkCompileErrors(vertexShader, "", "����");

	glShaderSource(fragmentShaderOrange, 1, &fragmentShader1Source, NULL);	//����ɫ��Դ�븳ֵ����ɫ��
	glCompileShader(fragmentShaderOrange);
	checkCompileErrors(vertexShader, "", "OrangeƬ��");

	glShaderSource(fragmentShaderYellow, 1, &fragmentShader2Source, NULL);	//����ɫ��Դ�븳ֵ����ɫ��
	glCompileShader(fragmentShaderYellow);
	checkCompileErrors(vertexShader, "", "YellowƬ��");

	glAttachShader(shaderProgramOrange, vertexShader);//����ɫ�����ص���ɫ�����������
	glAttachShader(shaderProgramOrange, fragmentShaderOrange);
	glLinkProgram(shaderProgramOrange);		//������ɫ������
	checkCompileErrors(shaderProgramOrange, "PROGRAM", "OrangeƬ��");

	glAttachShader(shaderProgramYellow, vertexShader);//����ɫ�����ص���ɫ�����������
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);		//������ɫ������
	checkCompileErrors(shaderProgramYellow, "PROGRAM", "OrangeƬ��");

	glDeleteShader(vertexShader);	//ɾ����ɫ������
	glDeleteShader(fragmentShaderOrange);
	glDeleteShader(fragmentShaderYellow);


	float firstTriangle[] = {
		   -0.9f, -0.5f, 0.0f,  // left 
		   -0.0f, -0.5f, 0.0f,  // right
		   -0.45f, 0.5f, 0.0f,  // top 
	};
	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	unsigned int VBOs[2], VAOs[2];//���㻺�����
	// 0. ���ƶ������鵽�����й�OpenGLʹ��
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);//���ɶ��㻺�����VBO(Vertex Buffer Object)����
#pragma region ��һ��������
	// 1. ��VAO
	glBindVertexArray(VAOs[0]);
	// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
#pragma endregion

#pragma region �ڶ���������
	// 1. ��VAO
	glBindVertexArray(VAOs[1]);
	// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
#pragma endregion

	// ѭ����Ⱦ
	// -----------
	while (!glfwWindowShouldClose(window))//���ָ�����ڵĹرձ�־�����GLFW�Ƿ�Ҫ���˳�
	{
		// ����
		processInput(window);//�������
		// ��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//�����Ļ���õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������

		//��Ⱦһ������ʱҪʹ����ɫ������
		glUseProgram(shaderProgramOrange);
		//����������
		glBindVertexArray(VAOs[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);//���ƶ������� ��ʼ���� ����
		
		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAOs[1]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);//���ƶ������� ��ʼ���� ����
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);//������ɫ������
		glfwPollEvents();		//��鴥���¼��������ûص�����
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgramOrange);
	glDeleteProgram(shaderProgramYellow);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	//��ֹ�����֮ǰ���������glfw��Դ��
	// ------------------------------------------------------------------
	glfwTerminate();//���glfw��Դ��
	return 0;
}

// �����������룺��ѯGLFW�Ƿ���/�ͷ�����صļ�����������Ӧ�ķ�Ӧ
// ���������߼���
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//���ESC��GLFW_KEY_ESCAPE���������£�GLFW_PRESS��
		glfwSetWindowShouldClose(window, true);	//�رմ���
}
// ���ڴ�С�仯�ص�����
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//cout << "���ڴ�С�ı�" << width << "  " << height << endl;
	glViewport(0, 0, width, height);//ȷ���ӿ�ƥ���µĴ��ڳߴ磻
}
///<summary>������ʧ����־</summary>
void checkCompileErrors(unsigned int shader, string type, string name)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout << type << "��ɫ������ʧ�� " << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cout << name << "��ɫ����������ʧ�� " << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
		}
	}
}
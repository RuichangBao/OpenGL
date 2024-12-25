//����Ļ��Ʒ�ʽ����
#include "TexturesSurround.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stbimage/stb_image.h>
#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include<iostream>

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TexturesTest", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "����GLFW����ʧ��" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//ʹָ�����ڵ�������Ϊ�����̵߳ĵ�ǰ�����ġ�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//���ô��ڴ�С�仯�Ļص�����

	// glad: ��������OpenGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "��ʼ��GLAD(OpenGL����ָ�����)ʧ��" << endl;
		return -1;
	}
	// ����������shader����
	Shader ourShader("shader/Vertex.shader", "shader/Fragment.shader");

	float vertices[] = {
		-0.5f, 0.5f, 0.0f, 0.0f, 2.0f, // ���Ͻ�
		0.5f, 0.5f, 0.0f, 2.0f, 2.0f,	// ���Ͻ�
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // ���½�
		0.5f, -0.5f, 0.0f, 2.0f, 0.0f,	// ���½�
	};

	unsigned int indexs[] = {
		0,1,2,
		1,2,3
	};

	unsigned int  VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);//���ɶ����������VAO �� OpenGL �������洢�������Ե����ã��Ա��ں�������ʱ���ٷ�����Щ���ԡ�
	glGenBuffers(1, &VBO);		//���ɶ��㻺�����VBO(Vertex Buffer Object)����
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);//�󶨶����������
	// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	//�� VBO �����䶥������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);


	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//���ö�������
	// ��������
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//���ش���ͼƬ
	//-------------------------
	unsigned int texture;
	glGenTextures(1, &texture);//�����������
	glBindTexture(GL_TEXTURE_2D, texture); // ���������
	//����������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//�ظ�ƽ��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//�ظ�ƽ��
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);//�����ظ�ƽ��
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);//�����ظ�ƽ��
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//��Ե����
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//��Ե����

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);//��Ե��ɫ���
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);//��Ե��ɫ���
	//float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // ��ɫ
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//������Сʱ��Ĺ��˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//����ͼ�񣬴�������������ͼ
	int width, height, nrChannels;
	unsigned char* data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		//ͼ�������ϴ��� GPU ����������ĸ�ʽ������
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//���� Mipmap ����㼶
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "ͼƬ����ʧ��" << endl;
	}
	stbi_image_free(data);//�ͷź��������ͼ���ڴ�

	//ѭ����Ⱦ
	while (!glfwWindowShouldClose(window))
	{
		//������
		processInput(window);

		// ��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//�����Ļ���õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������

		// bind Texture
		glBindTexture(GL_TEXTURE_2D, texture);

		//��Ⱦһ������ʱҪʹ����ɫ������
		ourShader.use();
		glBindVertexArray(VAO);//�󶨶�������
		//glDrawArrays(GL_TRIANGLES, 0, 3);//���ƶ�������
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//����ͼԪ����

		glfwSwapBuffers(window);//������ɫ������
		glfwPollEvents();		//��鴥���¼��������ûص�����
	}

	//��ֹ�����֮ǰ���������glfw��Դ��
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	//��ֹ�����֮ǰ���������glfw��Դ��
	glfwTerminate();
	return 0;
}

//��������
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// ���ڴ�С�����仯��ʱ�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
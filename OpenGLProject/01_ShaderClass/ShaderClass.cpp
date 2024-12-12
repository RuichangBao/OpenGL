//https://github.com/LearnOpenGL-CN/LearnOpenGL-CN/blob/new-theme/docs/01%20Getting%20started/05%20Shaders.md
#include<iostream>
#include<LearnOpenGL/Shader_s.h>
#include"ShaderClass.h"


using namespace std;

int main()
{
	// glfw: ��ʼ������
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//OpenGL�������ļ������� ����ģʽ
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//�����޸Ĵ��ڴ�С
#endif
	// glfw �������ڶ���
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ShaderClass", NULL, NULL);
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
	Shader ourShader("shader/Vertex.shader", "shader/Fragment.shader");
	float vertices[] = {//����λ�á���ɫ��Ϣ
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	unsigned int VBO, VAO;//���㻺�����
	// 0. ���ƶ������鵽�����й�OpenGLʹ��
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);//���ɶ��㻺�����VBO(Vertex Buffer Object)����
	// 1. ��VAO
	glBindVertexArray(VAO);

	// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);//���ö�������
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
		ourShader.use();
		//����������
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);//���ƶ������� ��ʼ���� ����

		glfwSwapBuffers(window);//������ɫ������
		glfwPollEvents();		//��鴥���¼��������ûص�����
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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

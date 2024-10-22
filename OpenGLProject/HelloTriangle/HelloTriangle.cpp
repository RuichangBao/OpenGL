//https://github.com/LearnOpenGL-CN/LearnOpenGL-CN/blob/new-theme/docs/01%20Getting%20started/04%20Hello%20Triangle.md
#include "HelloTriangle.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

int main()
{
	// glfw: ��ʼ������
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//OpenGL�������ļ������� ����ģʽ
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//�������޸Ĵ��ڴ�С
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// glfw �������ڶ���
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HelloTriangle", NULL, NULL);
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

#pragma region ������ɫ��
	//---------------������ɫ�� Start---------------------
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//����������ɫ������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//����ɫ��Դ�븳ֵ����ɫ��
	glCompileShader(vertexShader);
	int successVertex;
	char infoLogVertex[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVertex);//��ȡ��ɫ��״̬
	if (!successVertex)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLogVertex);
		cout << "������ɫ���������\n" << infoLogVertex << endl;
	}
	//---------------������ɫ�� End---------------------
#pragma endregion

#pragma region Ƭ����ɫ��
	//---------------Ƭ����ɫ�� Start-------------------
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//����������ɫ������
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//����ɫ��Դ�븳ֵ����ɫ��
	glCompileShader(fragmentShader);
	int successFragment;
	char infoLogFragment[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFragment);//��ȡ��ɫ��״̬
	if (!successFragment)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFragment);
		cout << "Ƭ����ɫ���������\n" << infoLogFragment << endl;
	}
	//---------------Ƭ����ɫ�� End---------------------
#pragma endregion

#pragma region ��ɫ���������
	//---------------��ɫ��������� Start-------------------
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();	//������ɫ���������
	glAttachShader(shaderProgram, vertexShader);//����ɫ�����ص���ɫ�����������
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);		//������ɫ������
	int shaderProgramSuccess;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderProgramSuccess);
	if (!shaderProgramSuccess)
	{
		cout << "������ɫ���������\n" << shaderProgramSuccess << endl;
		return -1;
	}
	glUseProgram(shaderProgram);	//������ɫ���������
	glDeleteShader(vertexShader);	//ɾ����ɫ������
	glDeleteShader(fragmentShader);
	//---------------��ɫ��������� End-------------------
#pragma endregion

	float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
	};
	unsigned int VBO, VAO;//���㻺�����
	// 0. ���ƶ������鵽�����й�OpenGLʹ��
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);//���ɶ��㻺�����VBO(Vertex Buffer Object)����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//�󶨶��㻺�����
	// 1. ��VAO
	glBindVertexArray(VAO);
	// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ѭ����Ⱦ
	// -----------
	while (!glfwWindowShouldClose(window))//���ָ�����ڵĹرձ�־�����GLFW�Ƿ�Ҫ���˳�
	{
		// ����
		processInput(window);//�������
		// ��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//�����Ļ���õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);//������ɫ������
		glfwPollEvents();		//��鴥���¼��������ûص�����
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	//��ֹ�����֮ǰ���������glfw��Դ��
	// ------------------------------------------------------------------
	glfwTerminate();//���glfw��Դ��
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
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
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.

	//cout << "���ڴ�С�ı�" << width << "  " << height << endl;
	glViewport(0, 0, width, height);//ȷ���ӿ�ƥ���µĴ��ڳߴ磻
}


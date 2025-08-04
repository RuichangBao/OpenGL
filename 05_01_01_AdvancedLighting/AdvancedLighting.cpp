//https://learnopengl-cn.github.io/05%20Advanced%20Lighting/01%20Advanced%20Lighting
#include "AdvancedLighting.h"
#include<iostream>
#include <learnopengl/shader.h>

using namespace std;

int main()
{
	//glfw: ��ʼ������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//OpenGL�������ļ������� ����ģʽ
	//glfwWindowHint(GLFW_SAMPLES, 16);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//�����޸Ĵ��ڴ�С
#endif
	// glfw �������ڶ���
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "AntiAliasingOffscreen", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "����GLFW����ʧ��" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//ʹָ�����ڵ�������Ϊ�����̵߳ĵ�ǰ�����ġ�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//���ô��ڴ�С�仯�Ļص�����
	glfwSetCursorPosCallback(window, mouse_callback);	//�����������ص�����
	glfwSetScrollCallback(window, scroll_callback);		//����������
	//����GLFW�������ǵ����
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: ��������OpenGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "��ʼ��GLAD(OpenGL����ָ�����)ʧ��" << endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);//����͸���Ȳ���
	//glEnable(GL_MULTISAMPLE); //�����ز�������ݣ���ĳЩ����������Ĭ�����ã�������ȫ�����ã����ʼ��������ȷ��
	// ����������shader����
	Shader shader("shader/Vertex.shader", "shader/Fragment.shader");
	unsigned int planVBO, planeVAO;
	glGenVertexArrays(1, &planeVAO);//���ɶ����������VAO �� OpenGL �������洢�������Ե����ã��Ա��ں�������ʱ���ٷ�����Щ���ԡ�
	glGenBuffers(1, &planVBO);		//���ɶ��㻺�����VBO(Vertex Buffer Object)����

	glBindVertexArray(planeVAO);//�󶨶����������
	// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	//�� VBO �����䶥������
	glBindBuffer(GL_ARRAY_BUFFER, planVBO);//GL_ARRAY_BUFFER:���㻺�����İ�Ŀ��
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	// λ������
	glEnableVertexAttribArray(0);	//���ö�������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	shader.use();
	//ѭ����Ⱦ
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//������
		processInput(window);
		// ��Ⱦ
		glEnable(GL_DEPTH_TEST); // ������Ȳ��ԣ���Ⱦ��Ļ�ռ��ı���ʱ���ã�
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//�����Ļ���õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT:�����Ȼ���
		glEnable(GL_DEPTH_TEST);

		//��Ⱦһ������ʱҪʹ����ɫ������
		shader.use();
		//ģ�;���
		glm::mat4 model = glm::mat4(1);
		float angle = glm::radians(45.0f);
		model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
		//�۲����
		glm::mat4 view = camera.GetViewMatrix();

		//ͶӰ����
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		shader.setMat4("view", view);
		shader.setMat4("model", model);
		shader.setMat4("projection", projection);

		glBindVertexArray(planeVAO);//�󶨶�������
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);//������ɫ������
		glfwPollEvents();		//��鴥���¼��������ûص�����
	}

	//��ֹ�����֮ǰ���������glfw��Դ��
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planVBO);
	//��ֹ�����֮ǰ���������glfw��Դ��
	glfwTerminate();

	return 0;
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

/// <summary>�������</summary>
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}
//�����ֻص�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
// ���ڴ�С�仯�ص�����
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//cout << "���ڴ�С�ı�" << width << "  " << height << endl;
	glViewport(0, 0, width, height);//ȷ���ӿ�ƥ���µĴ��ڳߴ磻
}
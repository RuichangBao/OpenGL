//https://github.com/LearnOpenGL-CN/LearnOpenGL-CN/blob/new-theme/docs/02%20Lighting/03%20Materials.md
#include "Materials.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CoordinateSystemsDepth", NULL, NULL);
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glad: ��������OpenGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "��ʼ��GLAD(OpenGL����ָ�����)ʧ��" << endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);//����͸���Ȳ���
	// ����������shader����
	Shader materialShader("shader/MaterialVertex.shader", "shader/MaterialFragment.shader");
	Shader cubeShader("shader/Vertex.shader", "shader/Fragment.shader");

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);//���ɶ����������VAO �� OpenGL �������洢�������Ե����ã��Ա��ں�������ʱ���ٷ�����Щ���ԡ�
	glGenBuffers(1, &VBO);		//���ɶ��㻺�����VBO(Vertex Buffer Object)����

	// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	//�� VBO �����䶥������
	//����ģ��������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//GL_ARRAY_BUFFER:���㻺�����İ�Ŀ��
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);//�󶨶����������
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//���ö�������
	// ��������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);	//���ö�������

	//���յ�������
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//���ö�������

	//ѭ����Ⱦ
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwFocusWindow(window);//ǿ�ƻ�ȡ����
		//������
		processInput(window);

		// ��Ⱦ
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//�����Ļ���õ���ɫ
		//glClear(GL_COLOR_BUFFER_BIT);//�����ɫ������
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT:�����Ȼ���

		materialShader.use();
		materialShader.setVec3("light.position", lightPos);
		materialShader.setVec3("viewPos", camera.Position);
		glm::vec3 lightColor;
		lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
		lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
		lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3));
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		materialShader.setVec3("light.ambient", ambientColor);
		materialShader.setVec3("light.diffuse", diffuseColor);
		materialShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//����material�ṹ������
		materialShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);//������
		materialShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		materialShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		materialShader.setFloat("material.shininess", 32);

		//ģ�;���
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(45, 45, 0));
		materialShader.setMat4("model", model);
		//�۲����
		glm::mat4 view = camera.GetViewMatrix();
		materialShader.setMat4("view", view);

		//ͶӰ����
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		materialShader.setMat4("projection", projection);

		glBindVertexArray(cubeVAO);//�󶨶�������
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//��Դ
		cubeShader.use();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // �ƹ�
		cubeShader.setMat4("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);//������ɫ������
		glfwPollEvents();		//��鴥���¼��������ûص�����
	}
	//��ֹ�����֮ǰ���������glfw��Դ��
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

	//��ֹ�����֮ǰ���������glfw��Դ��
	glfwTerminate();

	return 0;
}


//��������
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//���ESC��GLFW_KEY_ESCAPE���������£�GLFW_PRESS��
		glfwSetWindowShouldClose(window, true);	//�رմ���

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

//�Զ�����־���
void Print(glm::mat4 mat)
{
	Print(mat[0]);
	Print(mat[1]);
	Print(mat[2]);
	Print(mat[3]);
}

void Print(glm::vec4 vec)
{
	std::cout << " " << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << endl;
}



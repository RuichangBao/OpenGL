//https://github.com/LearnOpenGL-CN/LearnOpenGL-CN/blob/new-theme/docs/02%20Lighting/05%20Light%20casters.md
//ƽ�����ɵľ۹��
#include "LightCastersSpotSoft.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <stbimage/stb_image.h>

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LightingMapsDiffuse", NULL, NULL);
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
	Shader spotLightShader("shader/SpotLightVertex.shader", "shader/SpotLightFragment.shader");
	//Shader cubeShader("shader/Vertex.shader", "shader/Fragment.shader");

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//���ö�������
	// ��������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);	//���ö�������
	// ��������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);	//���ö�������

	unsigned int diffuseMap = loadTexture(FileSystem::getPath("Resources/Textures/container2.png").c_str());
	unsigned int specularMap = loadTexture(FileSystem::getPath("resources/textures/container2_specular.png").c_str());
	spotLightShader.use();
	spotLightShader.setInt("material.diffuse", 0);
	spotLightShader.setInt("material.specular", 1);
	//���ù���˥��
	spotLightShader.setFloat("light.constant", 1.0f);
	spotLightShader.setFloat("light.linear", 0.09f);
	spotLightShader.setFloat("light.quadratic", 0.032f);
	spotLightShader.setVec3("light.position", camera.Position);
	//�۹��
	spotLightShader.setVec3("light.direction", camera.Front);
	spotLightShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));	//��Բ׶
	//spotLightShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));//��Բ׶
	spotLightShader.setFloat("light.outerCutOff", glm::cos(glm::radians(15.0f)));//��Բ׶
	spotLightShader.setFloat("material.shininess", 32.0f);
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

		spotLightShader.use();
		spotLightShader.setVec3("viewPos", camera.Position);
		// ����ǿ������
		spotLightShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		spotLightShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		spotLightShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		// material properties

		spotLightShader.setVec3("light.direction", camera.Front);//���շ����ɹ�Դָ������
		//ģ�;���
		glm::mat4 model = glm::mat4(1.0f);
		//�۲����
		glm::mat4 view = camera.GetViewMatrix();
		spotLightShader.setMat4("view", view);

		//ͶӰ����
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		spotLightShader.setMat4("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glBindVertexArray(cubeVAO);//�󶨶�������
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			spotLightShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// glfw: ��������������ѯIO�¼���������/�ͷţ�����ƶ��ȣ�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//��ֹ�����֮ǰ���������glfw��Դ��
	glDeleteVertexArrays(1, &cubeVAO);
	//glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

	//��ֹ�����֮ǰ���������glfw��Դ��
	glfwTerminate();
	return 0;
}



unsigned int loadTexture(char const* path)
{
	// ���ش���һ����������
	unsigned int textureID;
	glGenTextures(1, &textureID);//������������

	//������������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//�ظ�ƽ��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//�ظ�ƽ��
	//������Сʱ��Ĺ��˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//����ͼ�񣬴�������������ͼ
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);// ����������
		//ͼ�������ϴ��� GPU �����������ĸ�ʽ������
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//���� Mipmap �����㼶
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ͼƬ����ʧ��" << std::endl;
	}
	stbi_image_free(data);//�ͷź��������ͼ���ڴ�
	return textureID;
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
void Print(glm::vec3 vec)
{
	std::cout << " " << vec.x << " " << vec.y << " " << vec.z << endl;
}
void Print(glm::vec4 vec)
{
	std::cout << " " << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << endl;
}
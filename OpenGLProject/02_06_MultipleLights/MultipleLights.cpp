//https://github.com/LearnOpenGL-CN/LearnOpenGL-CN/blob/new-theme/docs/02%20Lighting/05%20Light%20casters.md
//����ƹ�
#include "MultipleLights.h"
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
	Shader multipleLightsShader("shader/MultipleLightsVertex.shader", "shader/MultipleLightsFragment.shader");
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//���ö�������
	// ��������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);	//���ö�������
	// ��������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);	//���ö�������
	
	//�ƹ�������
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);//�󶨶����������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//���ö�������

	unsigned int diffuseMap = loadTexture(FileSystem::getPath("Resources/Textures/container2.png").c_str());
	unsigned int specularMap = loadTexture(FileSystem::getPath("resources/textures/container2_specular.png").c_str());
	
	multipleLightsShader.use();
	multipleLightsShader.setInt("material.diffuse", 0);
	multipleLightsShader.setInt("material.specular", 1);
	multipleLightsShader.setFloat("material.shininess", 32.0f);
	// ƽ�й�
	multipleLightsShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	multipleLightsShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	multipleLightsShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	multipleLightsShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// ���Դ1
	multipleLightsShader.setVec3("pointLights[0].position", pointLightPositions[0]);
	multipleLightsShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	multipleLightsShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	multipleLightsShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	multipleLightsShader.setFloat("pointLights[0].constant", 1.0f);
	multipleLightsShader.setFloat("pointLights[0].linear", 0.09f);
	multipleLightsShader.setFloat("pointLights[0].quadratic", 0.032f);
	// ���Դ 2
	multipleLightsShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	multipleLightsShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	multipleLightsShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	multipleLightsShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	multipleLightsShader.setFloat("pointLights[1].constant", 1.0f);
	multipleLightsShader.setFloat("pointLights[1].linear", 0.09f);
	multipleLightsShader.setFloat("pointLights[1].quadratic", 0.032f);
	// ���Դ 3
	multipleLightsShader.setVec3("pointLights[2].position", pointLightPositions[2]);
	multipleLightsShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	multipleLightsShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	multipleLightsShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	multipleLightsShader.setFloat("pointLights[2].constant", 1.0f);
	multipleLightsShader.setFloat("pointLights[2].linear", 0.09f);
	multipleLightsShader.setFloat("pointLights[2].quadratic", 0.032f);
	// ���Դ 4
	multipleLightsShader.setVec3("pointLights[3].position", pointLightPositions[3]);
	multipleLightsShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	multipleLightsShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	multipleLightsShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	multipleLightsShader.setFloat("pointLights[3].constant", 1.0f);
	multipleLightsShader.setFloat("pointLights[3].linear", 0.09f);
	multipleLightsShader.setFloat("pointLights[3].quadratic", 0.032f);
	//�۹��
	multipleLightsShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	multipleLightsShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	multipleLightsShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	multipleLightsShader.setFloat("spotLight.constant", 1.0f);
	multipleLightsShader.setFloat("spotLight.linear", 0.09f);
	multipleLightsShader.setFloat("spotLight.quadratic", 0.032f);
	multipleLightsShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	multipleLightsShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

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

		multipleLightsShader.use();
		multipleLightsShader.setVec3("viewPos", camera.Position);
		
		// �۹��
		multipleLightsShader.setVec3("spotLight.position", camera.Position);
		multipleLightsShader.setVec3("spotLight.direction", camera.Front);

		//ģ�;���
		glm::mat4 model = glm::mat4(1.0f);
		//�۲����
		glm::mat4 view = camera.GetViewMatrix();
		multipleLightsShader.setMat4("view", view);

		//ͶӰ����
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		multipleLightsShader.setMat4("projection", projection);
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
			multipleLightsShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//��Դ
		cubeShader.use();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			cubeShader.setMat4("model", model);
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
	// ���ش���һ���������
	unsigned int textureID;
	glGenTextures(1, &textureID);//�����������

	//����������
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
		glBindTexture(GL_TEXTURE_2D, textureID);// ���������
		//ͼ�������ϴ��� GPU ����������ĸ�ʽ������
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//���� Mipmap ����㼶
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

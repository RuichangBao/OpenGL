//https://learnopengl-cn.github.io/05%20Advanced%20Lighting/03%20Shadows/01%20Shadow%20Mapping/
//�����ͼ
#include "ShadowMappingSSP.h"
#include<iostream>
#include <stbimage/stb_image.h>
#include <learnopengl/filesystem.h>
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ShadowMappingSSP", NULL, NULL);
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
	//glEnable(GL_CULL_FACE);  // �����޳�����
	// ����������shader����
	Shader shader("shader/Vertex.shader", "shader/Fragment.shader");
	Shader simpleDepthShader("shader/ShadowMappingDepthVertex.shader", "shader/ShadowMappingDepthFragment.shader");
	//Shader debugDepthQuad("shader/DebugQuadVertex.shader", "shader/DebugQuadFragment.shader");

	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);//���ɶ����������VAO �� OpenGL �������洢�������Ե����ã��Ա��ں�������ʱ���ٷ�����Щ���ԡ�
	glGenBuffers(1, &planeVBO);		//���ɶ��㻺�����VBO(Vertex Buffer Object)����

	glBindVertexArray(planeVAO);//�󶨶����������
	// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	//�� VBO �����䶥������
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);//GL_ARRAY_BUFFER:���㻺�����İ�Ŀ��
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	// λ������
	glEnableVertexAttribArray(0);	//���ö�������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	unsigned int woodTexture = loadTexture(FileSystem::getPath("resources/textures/wood.png").c_str(), false);
	
	//���ͼ
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	//�����������
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//������С����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//����Ŵ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//ˮƽƽ��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//��ֱƽ��

	//�������������ΪFBO����Ȼ���
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);//����Ҫд֡����
	glReadBuffer(GL_NONE);//����Ҫ��֡����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader.use();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("shadowMap", 1);
	//debugDepthQuad.use();
	//debugDepthQuad.setInt("depthMap", 0);

	//ѭ����Ⱦ
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//������
		processInput(window);
		// ��Ⱦ
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//�����Ļ���õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT:�����Ȼ���
		//1. ��Ⱦ�������������ȣ��ӹ�ĽǶȣ�
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		//��Դʹ������ͶӰ����
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);//��������ͶӰ����
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		//������ת������Դ�ռ�ľ��� ���ս��������ͼ�ռ���
		lightSpaceMatrix = lightProjection * lightView;
		//�ӹ�Դ�ĽǶ���Ⱦ����
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		renderScene(simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//���ô���
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		// set light uniforms
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPos", lightPos);
		shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderScene(shader);


		//��Ⱦ���ͼ��quad���п��ӻ�����
		/*debugDepthQuad.use();
		debugDepthQuad.setFloat("near_plane", near_plane);
		debugDepthQuad.setFloat("far_plane", far_plane);*/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		//renderQuad();

		glfwSwapBuffers(window);//������ɫ������
		glfwPollEvents();		//��鴥���¼��������ûص�����
	}

	//��ֹ�����֮ǰ���������glfw��Դ��
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	//��ֹ�����֮ǰ���������glfw��Դ��
	glfwTerminate();

	return 0;
}
//��Ⱦ3D����
void renderScene(const Shader &shader)
{
	// floor
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// ������
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.setMat4("model", model);
	renderCube();
}

//��NDC����Ⱦ1x1 3D�����塣
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	//��ʼ���������Ҫ��
	if (cubeVAO == 0)
	{
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

//��NDC����Ⱦ1x1 XY�ı���
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
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
//gammaCorrection �Զ�����٤��У��
unsigned int loadTexture(char const* path, bool gammaCorrection = false)
{
	// ���ش���һ���������
	unsigned int textureID;
	glGenTextures(1, &textureID);//�����������

	//����ͼ�񣬴�������������ͼ
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;
		if (nrChannels == 1)
			dataFormat = GL_RED;
		else if (nrChannels == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrChannels == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}
		glBindTexture(GL_TEXTURE_2D, textureID);// ���������
		//ͼ�������ϴ��� GPU ����������ĸ�ʽ������
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		//���� Mipmap ����㼶
		glGenerateMipmap(GL_TEXTURE_2D); 

		//����������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//�ظ�ƽ��
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//�ظ�ƽ��
		//������Сʱ��Ĺ��˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "ͼƬ����ʧ��" << std::endl;
	}
	stbi_image_free(data);//�ͷź��������ͼ���ڴ�
	return textureID;
}
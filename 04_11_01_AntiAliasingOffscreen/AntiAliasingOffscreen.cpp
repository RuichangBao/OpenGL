//https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/11%20Anti%20Aliasing/
//�����
#include "AntiAliasingOffscreen.h"
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
	Shader screenShader("shader/ScreenVertex.shader", "shader/ScreenFragment.shader");
	unsigned int cubeVBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);//���ɶ����������VAO �� OpenGL �������洢�������Ե����ã��Ա��ں�������ʱ���ٷ�����Щ���ԡ�
	glGenBuffers(1, &cubeVBO);		//���ɶ��㻺�����VBO(Vertex Buffer Object)����

	glBindVertexArray(cubeVAO);//�󶨶����������
	// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	//�� VBO �����䶥������
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);//GL_ARRAY_BUFFER:���㻺�����İ�Ŀ��
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// λ������
	glEnableVertexAttribArray(0);	//���ö�������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
#pragma region ֡��������
	// ֡����
	unsigned int frameBufferVAO, frameBufferVBO;
	glGenVertexArrays(1, &frameBufferVAO);
	glGenBuffers(1, &frameBufferVBO);
	glBindVertexArray(frameBufferVAO);
	glBindBuffer(GL_ARRAY_BUFFER, frameBufferVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//֡��������
	//����MSAA framebuffer
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);//����֡����
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);//��֡����

	//����һ�����ز��������������(������)
	unsigned int textureColorBufferMultiSampled;
	glGenTextures(1, &textureColorBufferMultiSampled);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, SCR_WIDTH, SCR_HEIGHT, true);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);	//�������ӵ�֡������
	
	//Ϊ��Ⱥ�ģ�帽������һ����Ҳ�Ƕ������renderbuffer����
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);		// ��Ȼ����ģ�建��ʹ�õ���֡����(��ͨ֡��Ⱦ)
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);//�󶨶��ز�����Ⱦ����
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); //�������Ϣ��ģ�建����Ϣ���ӵ�֡����
	// ����֡���岢������˸��������
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "����::֡����:: ֡���岻����!" << endl;
	else
		cout << "������֡����" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//���õڶ�������֡������
	unsigned int intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
	//����һ����ɫ��������
	unsigned int screenTexture;
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//������С���˷�ʽ ˫���Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//����Ŵ���˷�ʽ ˫���Բ�ֵ
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	//����ֻ��Ҫһ����ɫ����

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "����::֡����:: ֡���岻����!" << endl;
	else
		cout << "������֡����" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

	screenShader.use();
	screenShader.setInt("screenTexture", 0);
	//ѭ����Ⱦ
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//������
		processInput(window);
		// ��Ⱦ
		// ��һ����׶�
		// ���ư󶨵�֡���������
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
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
		
		glBindVertexArray(cubeVAO);//�󶨶�������
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//�ڶ�����׶�
		// 2. blit���������(s)�������Ĳ�ɫ������м�FBO��ͼ��洢����Ļ������
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		// 3. �ó������Ӿ�Ч����Ϊ��������ͼ������Ⱦquad
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		//֡�������Ȱѻ�������һ������ Ȼ����ʾ����Ļ��
		screenShader.use();
		glBindVertexArray(frameBufferVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorBufferMultiSampled);	//ʹ����ɫ����������Ϊ�ı���ƽ�������
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);//������ɫ������
		glfwPollEvents();		//��鴥���¼��������ûص�����
	}

	//��ֹ�����֮ǰ���������glfw��Դ��
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &frameBufferVAO);
	glDeleteBuffers(1, &frameBufferVBO);
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

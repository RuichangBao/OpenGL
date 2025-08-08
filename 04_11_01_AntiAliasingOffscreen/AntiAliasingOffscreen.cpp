//https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/11%20Anti%20Aliasing/
//抗锯齿
#include "AntiAliasingOffscreen.h"
#include<iostream>
#include <learnopengl/shader.h>

using namespace std;
int main()
{
	//glfw: 初始化配置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//OpenGL的配置文件和属性 核心模式
	//glfwWindowHint(GLFW_SAMPLES, 16);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//允许修改窗口大小
#endif
	// glfw 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "AntiAliasingOffscreen", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "创建GLFW窗口失败" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//使指定窗口的上下文为调用线程的当前上下文。
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//设置窗口大小变化的回调函数
	glfwSetCursorPosCallback(window, mouse_callback);	//设置鼠标输入回调函数
	glfwSetScrollCallback(window, scroll_callback);		//设置鼠标滚轮
	//告诉GLFW捕获我们的鼠标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: 加载所有OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "初始化GLAD(OpenGL函数指针错误)失败" << endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);//开启透明度测试
	//glEnable(GL_MULTISAMPLE); //（多重采样抗锯齿）在某些驱动程序上默认启用，但并非全部启用，因此始终启用以确保
	// 构建并编译shader程序
	Shader shader("shader/Vertex.shader", "shader/Fragment.shader");
	Shader screenShader("shader/ScreenVertex.shader", "shader/ScreenFragment.shader");
	unsigned int cubeVBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);//生成顶点数组对象。VAO 在 OpenGL 中用来存储顶点属性的配置，以便在后续绘制时快速访问这些属性。
	glGenBuffers(1, &cubeVBO);		//生成顶点缓存对象VBO(Vertex Buffer Object)对象

	glBindVertexArray(cubeVAO);//绑定顶点数组对象
	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	//绑定 VBO 并传输顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);//GL_ARRAY_BUFFER:顶点缓冲对象的绑定目标
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// 位置属性
	glEnableVertexAttribArray(0);	//启用顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
#pragma region 帧缓冲配置
	// 帧缓冲
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

	//帧缓冲配置
	//配置MSAA framebuffer
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);//创建帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);//绑定帧缓冲

	//创建一个多重采样抗锯齿纹理附件(纹理附件)
	unsigned int textureColorBufferMultiSampled;
	glGenTextures(1, &textureColorBufferMultiSampled);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, SCR_WIDTH, SCR_HEIGHT, true);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);	//将纹理附加到帧缓冲上
	
	//为深度和模板附件创建一个（也是多采样）renderbuffer对象
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);		// 深度缓冲和模板缓冲使用的是帧缓冲(普通帧渲染)
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);//绑定多重采样渲染对象
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); //将深度信息和模板缓冲信息附加到帧缓冲
	// 创建帧缓冲并且添加了附件，检查
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "错误::帧缓冲:: 帧缓冲不完整!" << endl;
	else
		cout << "完整的帧缓冲" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//配置第二个后处理帧缓冲区
	unsigned int intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
	//创建一个颜色附件纹理
	unsigned int screenTexture;
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//纹理缩小过滤方式 双线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//纹理放大过滤方式 双线性插值
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	//我们只需要一个颜色缓冲

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "错误::帧缓冲:: 帧缓冲不完整!" << endl;
	else
		cout << "完整的帧缓冲" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

	screenShader.use();
	screenShader.setInt("screenTexture", 0);
	//循环渲染
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//输入检查
		processInput(window);
		// 渲染
		// 第一处理阶段
		// 绘制绑定到帧缓冲的纹理
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST); // 启用深度测试（渲染屏幕空间四边形时禁用）
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT:清除深度缓存
		glEnable(GL_DEPTH_TEST);

		//渲染一个物体时要使用着色器程序
		shader.use();
		//模型矩阵
		glm::mat4 model = glm::mat4(1);
		float angle = glm::radians(45.0f);
		model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
		//观察矩阵
		glm::mat4 view = camera.GetViewMatrix();

		//投影矩阵
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		shader.setMat4("view", view);
		shader.setMat4("model", model);
		shader.setMat4("projection", projection);
		
		glBindVertexArray(cubeVAO);//绑定顶点数组
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//第二处理阶段
		// 2. blit多采样缓冲(s)到正常的彩色缓冲的中间FBO。图像存储在屏幕纹理中
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		// 3. 用场景的视觉效果作为它的纹理图像来渲染quad
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		//帧缓冲是先把缓冲做成一张纹理 然后显示到屏幕上
		screenShader.use();
		glBindVertexArray(frameBufferVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorBufferMultiSampled);	//使用颜色附件纹理作为四边形平面的纹理
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);//交换颜色缓冲区
		glfwPollEvents();		//检查触发事件，并调用回调函数
	}

	//终止，清除之前分配的所有glfw资源。
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &frameBufferVAO);
	glDeleteBuffers(1, &frameBufferVBO);
	//终止，清除之前分配的所有glfw资源。
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

/// <summary>鼠标输入</summary>
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
//鼠标滚轮回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
// 窗口大小变化回调函数
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//cout << "窗口大小改变" << width << "  " << height << endl;
	glViewport(0, 0, width, height);//确保视口匹配新的窗口尺寸；
}

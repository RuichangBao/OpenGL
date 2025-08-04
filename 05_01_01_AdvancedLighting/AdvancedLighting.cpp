//https://learnopengl-cn.github.io/05%20Advanced%20Lighting/01%20Advanced%20Lighting
#include "AdvancedLighting.h"
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
	unsigned int planVBO, planeVAO;
	glGenVertexArrays(1, &planeVAO);//生成顶点数组对象。VAO 在 OpenGL 中用来存储顶点属性的配置，以便在后续绘制时快速访问这些属性。
	glGenBuffers(1, &planVBO);		//生成顶点缓存对象VBO(Vertex Buffer Object)对象

	glBindVertexArray(planeVAO);//绑定顶点数组对象
	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	//绑定 VBO 并传输顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, planVBO);//GL_ARRAY_BUFFER:顶点缓冲对象的绑定目标
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	// 位置属性
	glEnableVertexAttribArray(0);	//启用顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	shader.use();
	//循环渲染
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//输入检查
		processInput(window);
		// 渲染
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

		glBindVertexArray(planeVAO);//绑定顶点数组
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);//交换颜色缓冲区
		glfwPollEvents();		//检查触发事件，并调用回调函数
	}

	//终止，清除之前分配的所有glfw资源。
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planVBO);
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
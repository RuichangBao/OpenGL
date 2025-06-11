//https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/09%20Geometry%20Shader/
//爆炸效果
#include "GeometryShaderExploding.h"
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/model.h>
#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>


using namespace std;

int main()
{
	//glfw: 初始化配置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//OpenGL的配置文件和属性 核心模式
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//允许修改窗口大小
#endif
	// glfw 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ModelLoading", NULL, NULL);
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glad: 加载所有OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "初始化GLAD(OpenGL函数指针错误)失败" << endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);//开启透明度测试
	// 构建并编译shader程序
	//Shader shader("shader/Vertex.shader", "shader/Fragment.shader");
	Shader shader("shader/Vertex.shader", "shader/Fragment.shader","shader/Geometry.shader");
	
	Model nanosuit(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
	//循环渲染
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//glfwFocusWindow(window);//强制获取焦点
		//输入检查
		processInput(window);

		// 渲染
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//清空屏幕所用的颜色
		//glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT:清除深度缓存

		shader.use();

		//模型矩阵
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -15.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));	// it's a bit too big for our scene, so scale it down
		shader.setMat4("model", model);

		//观察矩阵
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);

		//投影矩阵
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		shader.setFloat("time", static_cast<float>(glfwGetTime()));

		nanosuit.Draw(shader);

		// glfw: 交换缓冲区和轮询IO事件（按键按/释放，鼠标移动等）
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//终止，清除之前分配的所有glfw资源。


	//终止，清除之前分配的所有glfw资源。
	glfwTerminate();
	return 0;
}


//键盘输入
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//如果ESC（GLFW_KEY_ESCAPE）键被按下（GLFW_PRESS）
		glfwSetWindowShouldClose(window, true);	//关闭窗口

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
//https://github.com/LearnOpenGL-CN/LearnOpenGL-CN/blob/new-theme/docs/01%20Getting%20started/04%20Hello%20Triangle.md
#include "HelloTriangle_Exercise2.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

int main()
{
	// glfw: 初始化配置
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//OpenGL的配置文件和属性 核心模式
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//允许修改窗口大小
#endif
	// glfw 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HelloTriangle_Exercise2", NULL, NULL);
	if (window == NULL)
	{
		cout << "创建GLFW窗口失败！" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//使指定窗口的上下文为调用线程的当前上下文。
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//设置窗口大小变化的回调函数

	// glad: 加载所有OpenGL函数指针
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "初始化GLAD(OpenGL函数指针错误)失败" << endl;
		return -1;
	}
	int success;
	char infoLog[512];
#pragma region 顶点着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建顶点着色器对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//把着色器源码赋值给着色器
	glCompileShader(vertexShader);	//编译顶点着色器
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//获取着色器状态
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "顶点着色器编译错误，\n" << infoLog << endl;
		return -1;
	}
#pragma endregion

#pragma region 片段着色器
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//创建片段着色器对象
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//把着色器源码赋值给着色器
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//获取着色器状态
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "片段着色器编译错误，\n" << infoLog << endl;
		return -1;
	}
#pragma endregion

#pragma region 着色器程序对象
	//---------------着色器程序对象 Start-------------------
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();	//创建着色器程序对象
	glAttachShader(shaderProgram, vertexShader);//把着色器加载到着色器程序对象上
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);		//链接着色器对象
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);//获取着色器程序状态
	if (!success)
	{
		cout << "链接着色器程序错误，\n" << success << endl;
		return -1;
	}
	//glUseProgram(shaderProgram);	//激活着色器程序对象
	glDeleteShader(vertexShader);	//删除着色器对象
	glDeleteShader(fragmentShader);
	//---------------着色器程序对象 End-------------------
#pragma endregion

	float firstTriangle[] = {
		   -0.9f, -0.5f, 0.0f,  // left 
		   -0.0f, -0.5f, 0.0f,  // right
		   -0.45f, 0.5f, 0.0f,  // top 
	};
	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	unsigned int VBOs[2], VAOs[2];//顶点缓冲对象
	// 0. 复制顶点数组到缓冲中供OpenGL使用
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);//生成顶点缓存对象VBO(Vertex Buffer Object)对象
#pragma region 第一个三角形
	// 1. 绑定VAO
	glBindVertexArray(VAOs[0]);
	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
#pragma endregion

#pragma region 第二个三角形
	// 1. 绑定VAO
	glBindVertexArray(VAOs[1]);
	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
#pragma endregion

	// 循环渲染
	// -----------
	while (!glfwWindowShouldClose(window))//检查指定窗口的关闭标志。检查GLFW是否被要求退出
	{
		// 输入
		processInput(window);//检测输入
		// 渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区

		//渲染一个物体时要使用着色器程序
		glUseProgram(shaderProgram);
		//绘制三角形
		glBindVertexArray(VAOs[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);//绘制顶点数组 初始索引 长度
		glBindVertexArray(VAOs[1]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);//绘制顶点数组 初始索引 长度
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);//交换颜色缓冲区
		glfwPollEvents();		//检查触发事件，并调用回调函数
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	//终止，清除之前分配的所有glfw资源。
	// ------------------------------------------------------------------
	glfwTerminate();//清空glfw资源。
	return 0;
}

// 处理所有输入：查询GLFW是否按下/释放了相关的键，并做出相应的反应
// 按下鼠标或者键盘
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//如果ESC（GLFW_KEY_ESCAPE）键被按下（GLFW_PRESS）
		glfwSetWindowShouldClose(window, true);	//关闭窗口
}
// 窗口大小变化回调函数
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//cout << "窗口大小改变" << width << "  " << height << endl;
	glViewport(0, 0, width, height);//确保视口匹配新的窗口尺寸；
}

//https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window/
#include "HelloWindows.h"
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
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//不允许修改窗口大小
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// glfw 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HelloWindows", NULL, NULL);
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
	//glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);//设置窗口大小
	// 循环渲染
	// -----------
	while (!glfwWindowShouldClose(window))//检查指定窗口的关闭标志。检查GLFW是否被要求退出
	{
		// 输入
		processInput(window);//检测输入
		// 渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);//交换颜色缓冲区
		glfwPollEvents();		//检查触发事件，并调用回调函数
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	//终止，清除之前分配的所有glfw资源。
	// ------------------------------------------------------------------
	glfwTerminate();//清空glfw资源。
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
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
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.

	//cout << "窗口大小改变" << width << "  " << height << endl;
	glViewport(0, 0, width, height);//确保视口匹配新的窗口尺寸；
}


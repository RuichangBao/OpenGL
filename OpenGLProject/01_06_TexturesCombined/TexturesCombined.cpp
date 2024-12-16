//https://github.com/LearnOpenGL-CN/LearnOpenGL-CN/blob/new-theme/docs/01%20Getting%20started/06%20Textures.md
#include "TexturesCombined.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stbimage/stb_image.h>
#include <learnopengl/filesystem.h>
#include <learnopengl/shader_s.h>

#include <iostream>


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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TexturesCombined", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "创建GLFW窗口失败" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//使指定窗口的上下文为调用线程的当前上下文。
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//设置窗口大小变化的回调函数

	// glad: 加载所有OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "初始化GLAD(OpenGL函数指针错误)失败" << endl;
		return -1;
	}

	// 构建并编译shader程序
	Shader ourShader("shader/Vertex.shader", "shader/Fragment.shader");

	// 设置顶点数据（和缓冲区）并配置顶点属性
	float vertices[] = {
		// 位置               // 颜色             // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // 右上角
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // 右下角
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 左下角
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // 左上角
	};
	unsigned int indices[] = {
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);//生成顶点数组对象。VAO 在 OpenGL 中用来存储顶点属性的配置，以便在后续绘制时快速访问这些属性。
	glGenBuffers(1, &VBO);		//生成顶点缓存对象VBO(Vertex Buffer Object)对象
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);//绑定顶点数组对象
	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	//绑定 VBO 并传输顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//GL_ARRAY_BUFFER:顶点缓冲对象的绑定目标
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//绑定 EBO 并传输索引数据
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//启用顶点属性
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//// 纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//创建加载纹理
	unsigned int texture1, texture2;
	//unsigned int texture2;
	int width, height, nrChannels;
	unsigned char* data;
	stbi_set_flip_vertically_on_load(true);//是否垂直翻转像素
	glGenTextures(1, &texture1);//生成纹理对象
	glBindTexture(GL_TEXTURE_2D, texture1); // 绑定纹理对象
	//设置纹理环绕
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//重复平铺
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//重复平铺
	//设置缩小时候的过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//加载图像，创建纹理生成贴图
	data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		//图像数据上传到 GPU 并定义纹理的格式和数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//生成 Mipmap 纹理层级
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "图片加载失败" << std::endl;
	}
	stbi_image_free(data);


	glGenTextures(1, &texture2);//生成纹理对象
	glBindTexture(GL_TEXTURE_2D, texture2); // 绑定纹理对象
	//设置纹理环绕
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//横坐标放大方式 重复平铺
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//重复平铺
	//设置缩小时候的过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//加载图像，创建纹理生成贴图
	data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		//图像数据上传到 GPU 并定义纹理的格式和数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//生成 Mipmap 纹理层级
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "图片加载失败" << std::endl;
	}
	stbi_image_free(data);

	ourShader.use(); // 不要忘记在设置制服之前激活/使用着色器
	GLint num = glGetUniformLocation(ourShader.ID, "num");
	glUniform1f(num, 0.2f);
	// 手动设置
	 /* glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	  glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);*/
	  //通过纹理类设置
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	//循环渲染
	while (!glfwWindowShouldClose(window))
	{
		//输入检查
		processInput(window);

		// 渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//渲染一个物体时要使用着色器程序
		ourShader.use();
		glBindVertexArray(VAO);//绑定顶点数组
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//绘制图元函数

		glfwSwapBuffers(window);//交换颜色缓冲区
		glfwPollEvents();		//检查触发事件，并调用回调函数
	}

	//终止，清除之前分配的所有glfw资源。
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	//终止，清除之前分配的所有glfw资源。
	glfwTerminate();
	return 0;
}

//处理输入
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// 窗口大小发生变化的时候调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

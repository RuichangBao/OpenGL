//https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/07%20Advanced%20Data/
//高级数据分批顶点
#include "AdvancedDataBatchVertices.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stbimage/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "BlendingSort", NULL, NULL);
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
	// 构建并编译shader程序
	Shader shader("shader/Vertex.shader", "shader/Fragment.shader");
	Shader skyboxShader("shader/SkyboxVertex.shader", "shader/SkyboxFragment.shader");

	//正方体
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);//生成顶点数组对象。VAO 在 OpenGL 中用来存储顶点属性的配置，以便在后续绘制时快速访问这些属性。
	glGenBuffers(1, &cubeVBO);		//生成顶点缓存对象VBO(Vertex Buffer Object)对象
	glBindVertexArray(cubeVAO);//绑定顶点数组对象
	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	//绑定 VBO 并传输顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);//GL_ARRAY_BUFFER:顶点缓冲对象的绑定目标


	/*//第一种方法
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);	//在GPU上分配内存并且填充数据
	//第二种方法
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), NULL, GL_STATIC_DRAW);	//在GPU上分配内存，并不填充数据
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertices), &cubeVertices);	//在GPU分配内存上填充数据
	
	//第三种方法
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), NULL, GL_STATIC_DRAW);	//在GPU上分配内存，并不填充数据
	//// 获取指针
	//void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	//// 复制数据到内存
	//memcpy(ptr, cubeVertices, sizeof(cubeVertices));
	//// 记得告诉OpenGL我们不再需要这个指针了
	//glUnmapBuffer(GL_ARRAY_BUFFER);
	
	// 位置属性
	glEnableVertexAttribArray(0);//启用顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));*/

	//第二种方法(分批顶点处理)
	// 1.0 用连个数组存数据
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesPos) + sizeof(cubeVerticesNormal), NULL, GL_STATIC_DRAW);	//在GPU上分配内存，并不填充数据
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVerticesPos), &cubeVerticesPos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVerticesPos), sizeof(cubeVerticesNormal), &cubeVerticesNormal);
	//
	//glEnableVertexAttribArray(0);//启用顶点属性
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)sizeof(cubeVerticesPos));
	//2.0 用一个数组存数据 上半部分顶点 下半部分法线
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), NULL, GL_STATIC_DRAW);	//在GPU上分配内存，并不填充数据
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertices) , &cubeVertices);

	glEnableVertexAttribArray(0);//启用顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(cubeVertices) / 2));


	//-----------天空盒子 Start----------------------------
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	//第一种方法
	//glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);	//在GPU上分配内存并且填充数据
	//第二种方法
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), NULL, GL_STATIC_DRAW);	//在GPU上分配内存，并不填充数据
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(skyboxVertices), &skyboxVertices);	//在GPU分配内存上填充数据
	//第三种方法
	//glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), NULL, GL_STATIC_DRAW);	//在GPU上分配内存，并不填充数据
	//// 获取指针
	//ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	//// 复制数据到内存
	//memcpy(ptr, skyboxVertices, sizeof(skyboxVertices));
	//// 记得告诉OpenGL我们不再需要这个指针了
	//glUnmapBuffer(GL_ARRAY_BUFFER);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<string>faces
	{
		FileSystem::getPath("resources/textures/skybox/right.jpg"),
		FileSystem::getPath("resources/textures/skybox/left.jpg"),
		FileSystem::getPath("resources/textures/skybox/top.jpg"),
		FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
		FileSystem::getPath("resources/textures/skybox/front.jpg"),
		FileSystem::getPath("resources/textures/skybox/back.jpg")
	};
	unsigned int cubemapTexture = loadCubemap(faces);
	//-----------天空盒子 End----------------------------


	// 加载贴图
	unsigned int floorTexture = loadTexture(FileSystem::getPath("resources/textures/metal.png").c_str());


	// 配置着色器
	shader.use();
	shader.setInt("skybox", 0);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	glfwFocusWindow(window);//强制获取焦点
	//循环渲染
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//输入检查
		processInput(window);

		// 渲染
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT:清除深度缓存

		//正常的绘制场景
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();

		shader.setMat4("model", model);
		shader.setMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);
		shader.setVec3("cameraPos", camera.Position);
		// 正方体
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);



		// 最后绘制skybox
		glDepthFunc(GL_LEQUAL);  //改变depth函数，当depth值等于depth buffer的内容时，depth测试通过(当深度值等于1的时候，就是没有绘制其他场景的时候才会绘制)
		skyboxShader.use();
		skyboxShader.setMat4("model", model);
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // 从视图矩阵中删除平移
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// 天空盒子
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); //当深度值小于当前深度缓冲中的值时候，才会绘制

		// glfw: 交换缓冲区和轮询IO事件（按键按/释放，鼠标移动等）
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//终止，清除之前分配的所有glfw资源。
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &skyboxVBO);

	//终止，清除之前分配的所有glfw资源。
	glfwTerminate();
	return 0;
}

unsigned int loadTexture(char const* path)
{
	// 加载创建一个纹理对象
	unsigned int textureID;
	glGenTextures(1, &textureID);//生成纹理对象

	//设置纹理环绕
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//重复平铺
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//重复平铺
	//设置缩小时候的过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//加载图像，创建纹理生成贴图
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
		glBindTexture(GL_TEXTURE_2D, textureID);// 绑定纹理对象
		//图像数据上传到 GPU 并定义纹理的格式和数据
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//生成 Mipmap 纹理层级
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "图片加载失败" << std::endl;
	}
	stbi_image_free(data);//释放函数分配的图像内存
	return textureID;
}

unsigned int loadCubemap(vector<std::string> faces)
{
	//创建一个纹理对象
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			//图像数据上传到 GPU 并定义纹理的格式和数据
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "立方体纹理加载失败，路径: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//缩小的过滤方式 最近邻采样，选择最近的纹理像素
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大的过滤方式 最近邻采样，选择最近的纹理像素
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}


//键盘输入
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//如果ESC（GLFW_KEY_ESCAPE）键被按下（GLFW_PRESS）
		glfwSetWindowShouldClose(window, true);	//关闭窗口

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
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

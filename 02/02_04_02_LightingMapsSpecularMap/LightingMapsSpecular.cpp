//https://github.com/LearnOpenGL-CN/LearnOpenGL-CN/blob/new-theme/docs/02%20Lighting/04%20Lighting%20maps.md
//高光反射贴图
#include "LightingMapsSpecular.h"
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
	//glfw: 初始化配置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//OpenGL的配置文件和属性 核心模式
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//允许修改窗口大小
#endif
	// glfw 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LightingMapsDiffuse", NULL, NULL);
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
	//Shader specularMapShader("shader/SpecularMapVertex.shader", "shader/SpecularMapFragment.shader");
	Shader specularMapShader("shader/SpecularMapVertex.shader", "shader/SpecularMapFragment2.shader");
	Shader cubeShader("shader/Vertex.shader", "shader/Fragment.shader");

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);//生成顶点数组对象。VAO 在 OpenGL 中用来存储顶点属性的配置，以便在后续绘制时快速访问这些属性。
	glGenBuffers(1, &VBO);		//生成顶点缓存对象VBO(Vertex Buffer Object)对象

	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	//绑定 VBO 并传输顶点数据
	//光照模型正方体
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//GL_ARRAY_BUFFER:顶点缓冲对象的绑定目标
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);//绑定顶点数组对象
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//启用顶点属性
	// 法线属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);	//启用顶点属性
	// 纹理坐标
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);	//启用顶点属性

	//光照的正方体
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	//启用顶点属性
	unsigned int diffuseMap = loadTexture(FileSystem::getPath("Resources/Textures/container2.png").c_str());
	unsigned int specularMap = loadTexture(FileSystem::getPath("Resources/Textures/container2_specular.png").c_str());
	specularMapShader.use();
	specularMapShader.setInt("material.diffuse", 0);
	specularMapShader.setInt("material.specular", 1);
	//循环渲染
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwFocusWindow(window);//强制获取焦点
		//输入检查
		processInput(window);

		// 渲染
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//清空屏幕所用的颜色
		//glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT:清除深度缓存

		specularMapShader.use();
		specularMapShader.setVec3("light.position", lightPos);
		specularMapShader.setVec3("viewPos", camera.Position);
		// 光照强度设置
		specularMapShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		specularMapShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		specularMapShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		// material properties
		specularMapShader.setFloat("material.shininess", 32.0f);

		//模型矩阵
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(45, 45, 0));
		specularMapShader.setMat4("model", model);
		//观察矩阵
		glm::mat4 view = camera.GetViewMatrix();
		specularMapShader.setMat4("view", view);

		//投影矩阵
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		specularMapShader.setMat4("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glBindVertexArray(cubeVAO);//绑定顶点数组
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//光源
		cubeShader.use();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.002f)); // 灯光
		cubeShader.setMat4("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);//交换颜色缓冲区
		glfwPollEvents();		//检查触发事件，并调用回调函数
	}
	//终止，清除之前分配的所有glfw资源。
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

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

//自定义日志输出


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
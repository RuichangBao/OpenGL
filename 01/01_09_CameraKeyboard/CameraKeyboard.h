#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;//unsigned 无符号
const unsigned int SCR_HEIGHT = 600;

//相机
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); //相机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//相机目标
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);//相机坐标上方向

float deltaTime = 0;//每帧的时长
float lastFrame = 0;//最后一帧的时间

//自定义日志输出
void Print(glm::mat4 mat);
void Print(glm::vec4 vec);
// 设置顶点数据（和缓冲区）并配置顶点属性
float vertices[] = {
   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
// 立方体的世界坐标
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};
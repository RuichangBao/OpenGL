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

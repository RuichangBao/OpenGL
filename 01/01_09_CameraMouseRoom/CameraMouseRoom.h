#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// settings
const unsigned int SCR_WIDTH = 800;//unsigned 无符号
const unsigned int SCR_HEIGHT = 600;

//相机
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); //相机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//相机目标
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);//相机坐标上方向

float deltaTime = 0;//每帧的时长
float lastFrame = 0;//最后一帧的时间

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float fov = 45.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//自定义日志输出
void Print(glm::mat4 mat);
void Print(glm::vec4 vec);

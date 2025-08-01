#include <GLFW/glfw3.h>
#include <learnopengl/camera.h>
// 屏幕宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//相机
Camera camera(glm::vec3(0.0f, 0.0f, 55.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);//窗口大小回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

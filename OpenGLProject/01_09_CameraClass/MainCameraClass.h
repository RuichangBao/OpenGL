#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// settings
const unsigned int SCR_WIDTH = 800;//unsigned �޷���
const unsigned int SCR_HEIGHT = 600;


float deltaTime = 0;//ÿ֡��ʱ��
float lastFrame = 0;//���һ֡��ʱ��

bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//�Զ�����־���
void Print(glm::mat4 mat);
void Print(glm::vec4 vec);
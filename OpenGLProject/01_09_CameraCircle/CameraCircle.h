#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;//unsigned �޷���
const unsigned int SCR_HEIGHT = 600;

//�Զ�����־���
void Print(glm::mat4 mat);
void Print(glm::vec4 vec);
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;//unsigned �޷���
const unsigned int SCR_HEIGHT = 600;

//���
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); //���λ��
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//���Ŀ��
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);//��������Ϸ���

float deltaTime = 0;//ÿ֡��ʱ��
float lastFrame = 0;//���һ֡��ʱ��

//�Զ�����־���
void Print(glm::mat4 mat);
void Print(glm::vec4 vec);

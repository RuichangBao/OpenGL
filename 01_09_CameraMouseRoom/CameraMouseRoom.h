#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// settings
const unsigned int SCR_WIDTH = 800;//unsigned �޷���
const unsigned int SCR_HEIGHT = 600;

//���
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); //���λ��
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//���Ŀ��
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);//��������Ϸ���

float deltaTime = 0;//ÿ֡��ʱ��
float lastFrame = 0;//���һ֡��ʱ��

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float fov = 45.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//�Զ�����־���
void Print(glm::mat4 mat);
void Print(glm::vec4 vec);

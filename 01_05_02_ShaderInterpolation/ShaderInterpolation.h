//��ֵ
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;//unsigned �޷���
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource =
"#version 330 core\n" 
"layout(location = 0) in vec3 aPos;\n"		//λ�ñ���������λ��ֵΪ 0 
"layout(location = 1) in vec3 aColor;\n"	//��ɫ����������λ��ֵΪ 1
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"	ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(ourColor, 1);\n"
"}\n";

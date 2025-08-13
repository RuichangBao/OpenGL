#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;//unsigned �޷���
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
	-1.0f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.0f,  0.5f, 0.5f
};


const char* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0)in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	vertexColor = vec4(0.5,0,0,1);\n"
"}\0";
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"	FragColor = vertexColor;\n"//�����ɫ�Ǻ�ɫ
"}\n";
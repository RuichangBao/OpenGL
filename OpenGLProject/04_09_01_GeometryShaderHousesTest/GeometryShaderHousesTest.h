#include <GLFW/glfw3.h>

// 屏幕宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

float points[] = {
	//位置		  //颜色
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
};

//float points[] = {
//	//位置		  //颜色
//	-0.5f,  0.5f, // top-left
//	 0.5f,  0.5f, // top-right
//	 0.5f, -0.5f, // bottom-right
//	-0.5f, -0.5f, // bottom-left
//};
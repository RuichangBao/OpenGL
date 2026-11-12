#pragma once
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <learnopengl/camera.h>
#include <string>
#include <vector>

using namespace std;

// 屏幕宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//相机
//Camera camera(glm::vec3(1.0f, 1.0f, 3.0f));
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//Camera camera(glm::vec3(2.0f, 0.0f, 0.0f));//立方体内部
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int loadTexture(char const* path);//加载纹理
unsigned int loadCubemap(vector<std::string> faces);//加载立方体纹理

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


//float vertices[] = {
//		0.25f, 0.25f, 0.0f,   // 右上角
//		0.25f, -0.25f, 0.0f,  // 右下角
//		-0.25f, -0.25f, 0.0f, // 左下角
//
//		-0.25f, -0.25f, 0.0f, // 左下角
//		-0.25f, 0.25f, 0.0f,   // 左上角
//		0.25f, 0.25f, 0.0f,   // 右上角
//};
//unsigned int indices[] = {
//	// 注意索引从0开始! 
//	// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
//	// 这样可以由下标代表顶点组合成矩形
//	0, 1, 3, // 第一个三角形
//	1, 2, 3  // 第二个三角形
//};

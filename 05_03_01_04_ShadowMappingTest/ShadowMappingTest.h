﻿#include <GLFW/glfw3.h>
#include <learnopengl/camera.h>
#include <learnopengl/shader.h>
// 屏幕宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//相机
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);//加载纹理


//在标准化设备坐标中填充整个屏幕的四边形的顶点属性。帧渲染坐标
//  (-1.1)----------------(1.1)
//     |                    |
//     |                    |
//     |                    |
//     |                    |
//     |                    |
//     |                    |
//     |                    |
//  (-1.-1)---------------(1.-1)

float planeVertices[] = {
    // positions        // texture Coords
    0.5f, -0.5f, 0.0f, 2.0f, 0.0f,
    0.5f,  0.5f, 0.0f, 2.0f, 2.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,

    0.5f,  0.5f, 0.0f, 2.0f, 2.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f ,
    -0.5f,  0.5f, 0.0f, 0.0f, 2.0f,
};

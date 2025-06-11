#include <GLFW/glfw3.h>
#include <learnopengl/camera.h>
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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);//窗口大小回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

unsigned int loadTexture(char const* path);//加载纹理


float cubeVertices[] = {
    // 位置 (x,y,z)        法线 (nx,ny,nz)     纹理坐标 (u,v)
    // 前面 (Front Face)
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 左下
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 右下
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 右上
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 右上
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 左上
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 左下

    // 后面 (Back Face)
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,

    // 左面 (Left Face)
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,

    // 右面 (Right Face)
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

     // 上面 (Top Face)
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

     // 下面 (Bottom Face)
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
};
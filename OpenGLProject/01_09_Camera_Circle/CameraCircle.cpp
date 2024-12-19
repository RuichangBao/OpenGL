#include "CameraCircle.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


using namespace std;

int main()
{
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);		//摄像机位置
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);	
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	cout << "Hello World!" << endl;
}

//自定义日志输出
void Print(glm::mat4 mat)
{
	Print(mat[0]);
	Print(mat[1]);
	Print(mat[2]);
	Print(mat[3]);
}

void Print(glm::vec4 vec)
{
	std::cout << " " << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << endl;
}

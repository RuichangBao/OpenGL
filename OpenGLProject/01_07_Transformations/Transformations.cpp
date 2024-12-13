#include "Transformations.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>

using namespace std;

int main()
{
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	// 译注：下面就是矩阵初始化的一个例子，如果使用的是0.9.9及以上版本
	// 下面这行代码就需要改为:
	// glm::mat4 trans = glm::mat4(1.0f)
	// 之后将不再进行提示
	//glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 trans = glm::mat4(1.0f); //单位矩阵
	Print(trans);

	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	Print(trans);

	vec = trans * vec;
	//std::cout << vec.x << vec.y << vec.z << std::endl;
	Print(vec);
	cout << "Hello World!" << endl;
	return 0;
}
void Print(glm::mat4 mat)
{
	Print(mat[0]);
	Print(mat[1]);
	Print(mat[2]);
	Print(mat[3]);
}

void Print(glm::vec4 vec)
{
	std::cout << "x:" << vec.x << "	y:" << vec.y << "	z:" << vec.z << "	w:" << vec.w << endl;
}
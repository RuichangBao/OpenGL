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
	// ��ע��������Ǿ����ʼ����һ�����ӣ����ʹ�õ���0.9.9�����ϰ汾
	// �������д������Ҫ��Ϊ:
	// glm::mat4 trans = glm::mat4(1.0f)
	// ֮�󽫲��ٽ�����ʾ
	//glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 trans = glm::mat4(1.0f); //��λ����
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
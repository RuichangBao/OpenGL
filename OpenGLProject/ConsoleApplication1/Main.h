#include<iostream>

using namespace std;

typedef int (*FuncPtr)(int, int);

// 定义一个函数，该函数匹配 typedef 的签名
int add(int a, int b) {
	return a + b;
}
int sub(int a, int b)
{
	return a - b;
}
#define glGenBuffers FuncPtr
#include<iostream>

using namespace std;

typedef int (*FuncPtr)(int, int);

// ����һ���������ú���ƥ�� typedef ��ǩ��
int add(int a, int b) {
	return a + b;
}
int sub(int a, int b)
{
	return a - b;
}
#define glGenBuffers FuncPtr
#include<iostream>

using namespace std;

typedef int (*FuncPtr)(int, int);
#define Add add
// ����һ���������ú���ƥ�� typedef ��ǩ��
int add(int a, int b) {
	return a + b;
}
int sub(int a, int b)
{
	return a - b;
}
#define funcPtr FuncPtr
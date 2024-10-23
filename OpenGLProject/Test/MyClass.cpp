#include "MyClass.h"

using namespace std;

MyClass::MyClass() :num(100), data(100)
{
	cout << "构造函数" << data<<" "<<num << endl;
}

// 成员函数实现
void MyClass::PrintMessage()
{
	std::cout << "Hello, World! Data: " << data << std::endl;
}
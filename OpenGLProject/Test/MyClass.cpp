#include "MyClass.h"

using namespace std;

MyClass::MyClass() :num(100), data(100)
{
	cout << "���캯��" << data<<" "<<num << endl;
}

// ��Ա����ʵ��
void MyClass::PrintMessage()
{
	std::cout << "Hello, World! Data: " << data << std::endl;
}
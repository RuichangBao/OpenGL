#include "MyClass.h"

using namespace std;

MyClass::MyClass()
{
	cout << "�޲ι���" << endl;
	MyClass::MyClass(0, 0);
}
MyClass::MyClass(int num, int data)
{
	cout << "�вι���" << endl;
	this->num = num;
	this->data = data;
}

void MyClass::ToString()
{
	cout << "num:" << num << "	data:" << data << endl;
}
void MyClass::Main()
{
	cout << "BBBBBBBBBB" << endl;;
}
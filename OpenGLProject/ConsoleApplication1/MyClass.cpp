#include "MyClass.h"

using namespace std;

MyClass::MyClass()
{
	cout << "无参构造" << endl;
	MyClass::MyClass(0, 0);
}
MyClass::MyClass(int num, int data)
{
	cout << "有参构造" << endl;
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
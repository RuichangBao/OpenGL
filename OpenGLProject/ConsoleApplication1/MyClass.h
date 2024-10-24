#include <iostream>
using namespace std;
class MyClass
{
	public:
		int num;
		MyClass();               // 构造函数声明
		MyClass(int num, int data);
		void ToString();// 成员函数声明
	private:
		int data;                 // 私有成员变量
//public:
//	int num;
//	int data;
//	MyClass()
//	{
//		cout << "无参数构造" << endl;
//		MyClass::MyClass(0, 0);
//	}
//
//	MyClass(int num, int data)
//	{
//		cout << "有参数构造" << endl;
//		this->num = num;
//		this->data = data;
//	}
//	void ToString()
//	{
//		cout << "num:" << num << "	data:" << data << endl;
//	}
};

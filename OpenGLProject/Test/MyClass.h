#include <iostream>

class MyClass
{
public:
	int num;
	MyClass();               // 构造函数声明
	MyClass(int num, int data);    
	void ToString();// 成员函数声明
private:
	int data;                 // 私有成员变量
	
};

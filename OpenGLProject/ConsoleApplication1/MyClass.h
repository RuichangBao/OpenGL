#include <iostream>
using namespace std;
class MyClass
{
	public:
		int num;
		MyClass();               // ���캯������
		MyClass(int num, int data);
		void ToString();// ��Ա��������
	private:
		int data;                 // ˽�г�Ա����
//public:
//	int num;
//	int data;
//	MyClass()
//	{
//		cout << "�޲�������" << endl;
//		MyClass::MyClass(0, 0);
//	}
//
//	MyClass(int num, int data)
//	{
//		cout << "�в�������" << endl;
//		this->num = num;
//		this->data = data;
//	}
//	void ToString()
//	{
//		cout << "num:" << num << "	data:" << data << endl;
//	}
};

#include<iostream>

using namespace std;

struct MyStruct
{
public:
	int num1;
	int num2;
	void ToString()
	{
		cout << "num1:" << num1 << "	" << "num2:" << num2 << endl;
	}
};

void TestFunc(const MyStruct& myStruct)
{
	cout << myStruct.num2 << endl;
}
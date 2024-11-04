#include "Main.h"
#include "MyClass.h"
#include <iostream>

using namespace std;

int main()
{
	/*MyClass* obj = new MyClass(1, 2);
	MyClass Test = *obj;
	MyClass Test2;
	Test.ToString();
	MyClass obj2 = MyClass(3, 4);
	obj2.ToString();
	int arr[5] = { 1,2,3,4,5, };
	int* p = arr;
	cout << size(arr)<<endl;
	int length = sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < length; i++)
	{
		cout << *p << endl;
		*p = *(p++);
	}
	MyClass::Main();*/


	//FuncPtr myFunc1 = add;
	glGenBuffers myFunc1 = add;
	// 调用函数
	int result = myFunc1(3, 4);
	std::cout << "Result: " << result << std::endl;  // 输出：Result: 7
	myFunc1 = sub;
	result = myFunc1(3, 4);
	std::cout << "Result: " << result << std::endl;  // 输出：Result: 7
	return 0;
}
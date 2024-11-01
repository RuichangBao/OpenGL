#include "MyClass.h"
#include <iostream>

using namespace std;

int main()
{
	MyClass* obj = new MyClass(1, 2);
	MyClass Test = *obj;
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
	MyClass::Main();
	return 0;
}

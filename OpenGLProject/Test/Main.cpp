#include "MyClass.h"
#include <iostream>

using namespace std;

int main()
{
	MyClass* obj=new MyClass(1,2);
	MyClass Test = *obj;
	Test.ToString();
	MyClass obj2 = MyClass(3,4);
	obj2.ToString();
	return 0;
}

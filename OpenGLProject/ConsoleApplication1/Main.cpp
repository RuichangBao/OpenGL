#include "Main.h"
#include "MyClass.h"
#include <iostream>
#include<vector>

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
	//funcPtr myFunc1 = add;
	//// 调用函数
	//int result = myFunc1(3, 4);
	//std::cout << "Result: " << result << std::endl;  // 输出：Result: 7
	//myFunc1 = sub;
	//result = myFunc1(3, 4);
	//std::cout << "Result: " << result << std::endl;  // 输出：Result: 7
	//std::cout << Add(5, 8) << std::endl;  // 输出：13
	/*float nums[10] = {1,2,3,4,5,6,7,8,9,10};
	float* f = nums;
	cout << f << endl;*/
	vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(2);
	vec.push_back(3);
	vector<int>::const_iterator it = vec.begin();
	while (it != vec.end())
	{
		cout << *it << " ";
		++it;
	}
	return 0;
}


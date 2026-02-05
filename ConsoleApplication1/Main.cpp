#include "Main.h"
#include "MyClass.h"
#include <iostream>
#include<vector>

using namespace std;



int main()
{
	float num[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << num[i][j] << endl;
		}
	}
	return 0;
}


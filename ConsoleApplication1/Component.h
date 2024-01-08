#pragma once

#include <iostream>

using namespace std;

class Component
{
public:
	
	int n_comp;
	int val_comp;

	Component(int n = -1, int val = -1)
		: n_comp(n), val_comp(val) { }

	void Print()
	{

		cout << "n_comp = " << n_comp << endl;
		cout << "val_comp = " << val_comp << endl;

	}
};

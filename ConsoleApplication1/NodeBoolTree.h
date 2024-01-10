#pragma once

#include "Interval.h"


class NodeBoolTree
{
public:
	unsigned int var; // номер переменной xi разложения

	int count; // мощность множества интервалов
	Interval** dnf; //множество интервалов

	BBV varsToBeConsidered; // маска на столбцы: 1 - надо рассматривать, 0 - не рассматриваются
	BBV rowsToBeConsidered; // маска на строки: 1 - надо рассматривать, 0 - не рассматриваются


	bool fixVal_0; //флаг, указывающий на фиксацию значения 0, для рассматриваемой перемемнной xi(var) 
	bool fixVal_1; //флаг, указывающий на фиксацию значения 1, для рассматриваемой перемемнной xi(var) 

	Interval Solution; // для сохранения решения


	NodeBoolTree(int len = 8, size_t cnt = 0)
		: Solution(len)
	{
		count = cnt;

		var = 0;

		fixVal_1 = false;
		fixVal_0 = false;

		if (cnt == 0)
		{
			dnf = nullptr;
		}

		else
		{
			dnf = new Interval * [count];
		}

	};

	
	
	~NodeBoolTree()
	{
		if (dnf)
		{
			delete[] dnf;
		}
	}
};


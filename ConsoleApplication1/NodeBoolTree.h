#pragma once

#include "Interval.h"


class NodeBoolTree
{
public:
	unsigned int var;// номер переменной xi разложения

	NodeBoolTree* lt, * rt;

	int count;// мощьность множества интервалов
	Interval** dnf;//множество интервалов
	BBV varsToBeConsidered;// маска на столбцы: 1 - надо рассматривать, 0 - не рассматриваются


	bool fixVal_0;//флаг, указывающий на фиксацию значения 0, для рассматриваемой перемемнной xi(var) 
	bool fixVal_1;//флаг, указывающий на фиксацию значения 1, для рассматриваемой перемемнной xi(var) 

	Interval Solution;// Для сохранения решения

	//NodeBoolTree(int len = 8) : Solution(len) {};

	NodeBoolTree(int len = 8, size_t cnt = 0)
		: Solution(len)
	{
		count = cnt;

		var = 0;

		lt = nullptr;
		rt = nullptr;

		fixVal_1 = false;
		fixVal_0 = false;

		if (cnt == 0)
		{
			//dnf = nullptr;
		}

		else
		{
			// динамически создать массив указателей размера `M`
			dnf = new Interval * [count];

			/*
			// динамически выделяем память размером `N` для каждой строки
			for (int i = 0; i < cnt; i++)
			{
				dnf[i] = new Interval(len);
			}
			*/
		}


		//cout << "Solution.length = " << Solution.length() << endl;

	};

	/*
	
	~NodeBoolTree()
	{
		if (lt)
		{
			delete lt;
		}

		if (rt)
		{
			delete rt;
		}

		if (dnf)
		{
		
			delete[] dnf;

		}


	}
	*/

	/*
	NodeBoolTree(size_t cnt, int len) : Solution(len) //добавили
	{
		count = cnt;

		// динамически создать массив указателей размера `M`
		dnf = new Interval* [count];

		// динамически выделяем память размером `N` для каждой строки
		for (int i = 0; i < cnt; i++) {
			dnf[i] = new Interval(len);
		}

		
		//cout << "Solution.length = " << Solution.length() << endl;
	
	};
	*/

};


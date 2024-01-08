#pragma once

#include "Interval.h"


class NodeBoolTree
{
public:
	unsigned int var;// ����� ���������� xi ����������

	NodeBoolTree* lt, * rt;

	int count;// ��������� ��������� ����������
	Interval** dnf;//��������� ����������
	BBV varsToBeConsidered;// ����� �� �������: 1 - ���� �������������, 0 - �� ���������������


	bool fixVal_0;//����, ����������� �� �������� �������� 0, ��� ��������������� ����������� xi(var) 
	bool fixVal_1;//����, ����������� �� �������� �������� 1, ��� ��������������� ����������� xi(var) 

	Interval Solution;// ��� ���������� �������

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
			// ����������� ������� ������ ���������� ������� `M`
			dnf = new Interval * [count];

			/*
			// ����������� �������� ������ �������� `N` ��� ������ ������
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
	NodeBoolTree(size_t cnt, int len) : Solution(len) //��������
	{
		count = cnt;

		// ����������� ������� ������ ���������� ������� `M`
		dnf = new Interval* [count];

		// ����������� �������� ������ �������� `N` ��� ������ ������
		for (int i = 0; i < cnt; i++) {
			dnf[i] = new Interval(len);
		}

		
		//cout << "Solution.length = " << Solution.length() << endl;
	
	};
	*/

};


#pragma once

#include "Interval.h"


class NodeBoolTree
{
public:
	unsigned int var; // ����� ���������� xi ����������

	int count; // �������� ��������� ����������
	Interval** dnf; //��������� ����������

	BBV varsToBeConsidered; // ����� �� �������: 1 - ���� �������������, 0 - �� ���������������
	BBV rowsToBeConsidered; // ����� �� ������: 1 - ���� �������������, 0 - �� ���������������


	bool fixVal_0; //����, ����������� �� �������� �������� 0, ��� ��������������� ����������� xi(var) 
	bool fixVal_1; //����, ����������� �� �������� �������� 1, ��� ��������������� ����������� xi(var) 

	Interval Solution; // ��� ���������� �������


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


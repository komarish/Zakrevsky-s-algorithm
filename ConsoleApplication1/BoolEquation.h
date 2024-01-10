#pragma once

#include "Interval.h"
#include "NodeBoolTree.h"




//����� ��� �������� ��������� ���������� ���������
class BoolEquation {

	Interval** setIntevals; //��������� ����������
	int num_rows; //���������� �����
	int count_var; //���������� ����������

public:


	int getNumRows()
	{
		return num_rows;
	}

	int getCountVar()
	{
		return count_var;
	}

	Interval** getSetIntevals()
	{
		return setIntevals;
	}

	void readFromPla(string file_name);
	void PrintEquation();
	bool isSolution(Interval Solution);
	BoolEquation();
	~BoolEquation();
};


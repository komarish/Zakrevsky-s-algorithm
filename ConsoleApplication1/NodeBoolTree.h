#pragma once

#include "Interval.h"


class NodeBoolTree
{
public:
	NodeBoolTree* lt, * rt;
	Interval** dnf;//��������� ����������
	int count;// ��������� ��������� ����������
	BBV mask;//
	unsigned int var;// ����� ���������� xi ����������
	bool fixVal_1;//����, ����������� �� �������� �������� 1, ��� ��������������� ����������� xi(var) 
	bool fixVal_0;//����, ����������� �� �������� �������� 0, ��� ��������������� ����������� xi(var) 
	Interval Solution;// ��� ���������� �������
};


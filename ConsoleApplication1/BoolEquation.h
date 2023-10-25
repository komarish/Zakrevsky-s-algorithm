#pragma once

#include "Interval.h"
#include "NodeBoolTree.h"


class BoolEquation {

	NodeBoolTree** dnf;//��������� ����������
	Interval* root;//������ ���������
	int count; //���������� ����������
public:
	Interval FindRoot();
	void CreateTree();
	void InitEquation(Interval* set, size_t size);
	void PrintEquation();
};


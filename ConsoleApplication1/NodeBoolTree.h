#pragma once

#include "Interval.h"


class NodeBoolTree
{
public:
	NodeBoolTree* lt, * rt;
	Interval** dnf;//множество интервалов
	int count;// мощьность множества интервалов
	BBV mask;//
	unsigned int var;// номер переменной xi разложения
	bool fixVal_1;//флаг, указывающий на фиксацию значения 1, для рассматриваемой перемемнной xi(var) 
	bool fixVal_0;//флаг, указывающий на фиксацию значения 0, для рассматриваемой перемемнной xi(var) 
	Interval Solution;// Для сохранения решения
};


#pragma once

#include "Interval.h"
#include "NodeBoolTree.h"


class BoolEquation {

	NodeBoolTree** dnf;//множество интервалов
	Interval* root;//Корень уравнения
	int count; //количество конъюнкций
public:
	Interval FindRoot();
	void CreateTree();
	void InitEquation(Interval* set, size_t size);
	void PrintEquation();
};


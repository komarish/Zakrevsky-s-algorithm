#pragma once

#include "Interval.h"
#include "NodeBoolTree.h"




//Класс для хранения множества интервалов уравнения
class BoolEquation {

	Interval** setIntevals; //множество интервалов
	int num_rows; //количество строк
	int count_var; //количество переменных

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


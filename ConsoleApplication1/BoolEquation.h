#pragma once

#include "Interval.h"
#include "NodeBoolTree.h"





class BoolEquation {

	Interval** setIntevals;
	int num_rows;
	int count_var;
	
	//NodeBoolTree** dnf;//множество интервалов
	//Interval* root;//Корень уравнения
	//int count; //количество конъюнкций
public:
	//Interval FindRoot();
	//void CreateTree();

	BoolEquation();

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

	//void InitEquation(Interval* set, size_t size);
	void PrintEquation()
	{
	
		// Выводим считанную матрицу
		std::cout << "MATRIX" << std::endl;
		std::cout << "num_rows: " << num_rows << std::endl;
		std::cout << "count_var:" << count_var << std::endl;

		for (int ix = 0; ix < num_rows; ix++)
		{
			cout << (string)(*setIntevals[ix]) << endl;
		}
	
	};

	~BoolEquation()
	{
		if (setIntevals)
		{
			for (int ix = 0; ix < num_rows; ix++)
			{
				delete setIntevals[ix];
				setIntevals[ix] = nullptr;
			}
		}
	}



	bool isSolution(Interval Solution);

};


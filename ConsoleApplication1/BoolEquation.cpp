#include "BoolEquation.h"


#include <iostream>

#include <fstream>
#include <string>

#include <exception> 



bool BoolEquation::isSolution(Interval Solution)
{
	if (!setIntevals)
	{
		return false;
	}

	for (int i = 0; i < num_rows; i++)
	{
		if (!(setIntevals[i]->isOrthogonal(Solution)))
		{
			//cout << "setIntevals[i]: " << setIntevals[i] << endl;
			//cout << "Solution:       " << (string)Solution << endl;
			//cout << "isOrthogonal:   " << setIntevals[i]->isOrthogonal(Solution) << endl;

			return false;
		}
	}

	return true;

}



BoolEquation::BoolEquation()
{
	setIntevals = nullptr;
	num_rows = 0;
	count_var = 0;
}


void BoolEquation::readFromPla(string file_name)
{
	if (setIntevals)
	{
		for (int ix = 0; ix < num_rows; ix++)
		{
			delete setIntevals[ix];
			setIntevals[ix] = nullptr;
		}
	}










	ifstream input(file_name);

	if (input.is_open()) {

		string line;
		//int num_rows;
		//int num_inputs;
		int num_outputs;

		while (getline(input, line)) {
			// Пропускаем комментарии
			if (line[0] == '#') continue;

			line.erase(line.find_last_not_of(" \n\r\t") + 1);

			// Считываем информацию о входах, выходах и матрице
			if (line.substr(0, 2) == ".i")
			{
				// Извлекаем количество входов
				count_var = std::stoi(line.substr(3));
				//std::cout << "num_inputs: " << num_inputs << std::endl;
			}
			else if (line.substr(0, 2) == ".o")
			{
				// Извлекаем количество выходов
				num_outputs = std::stoi(line.substr(3));
				//std::cout << "num_outputs: " << num_outputs << std::endl;
			}
			else if (line.substr(0, 2) == ".p")
			{
				// Извлекаем количество строк матрицы
				num_rows = std::stoi(line.substr(3));
				//std::cout << "num_rows: " << num_rows << std::endl;


				setIntevals = new Interval * [num_rows];

				// Считываем значения матрицы
				for (int i = 0; i < num_rows; i++)
				{

					try
					{
						getline(input, line);


						if (line.substr(0, 2) == ".e")
						{
							//cout << "iii = " << i << "   " << line << endl;
							if (i < num_rows)
							{
								//std::cerr << "Incorrect PLA" << file_name << std::endl;

								exception e("Incorrect PLA");

								throw e;
							}
						}
					}

					catch (exception& e)
					{
						cout << e.what() << endl;
						exit(3);
					}

					line.erase(line.find_last_not_of(" \n\r\t") + 1);


					int length = line.length();

					char* char_array = new char[length + 1];

					// copying the contents of the 
					// string to char array 
					//strcpy_s(char_array, sizeof(char_array), line.c_str());

					for (int cix = 0; cix < count_var; cix++)
					{
						char_array[cix] = line[cix];
					}

					char_array[count_var] = '\0';

					setIntevals[i] = new Interval(char_array);

					//cout << "i=" << i << endl;

					//cout << (string)(*setIntevals[i]) << endl;

					delete[] char_array;

				}

				
			}
		}




		input.close();
	}
	else
	{
		std::cerr << "File not open" << file_name << std::endl;
	}








}


/*
void BoolEquation::InitEquation(Interval* set, size_t size)
{
	dnf = NULL;
	root = NULL;
	count = 0; //количество конъюнкций

	if (set)
	{



	}


}
Interval BoolEquation::FindRoot()
{
	Interval I("1-----");
	return I;
}
void BoolEquation::CreateTree()
{



}
void BoolEquation::PrintEquation()
{
}
*/


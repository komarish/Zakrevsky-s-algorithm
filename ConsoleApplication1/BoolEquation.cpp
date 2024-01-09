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
			}
			else if (line.substr(0, 2) == ".o")
			{
				// Извлекаем количество выходов
				num_outputs = std::stoi(line.substr(3));
			}
			else if (line.substr(0, 2) == ".p")
			{
				// Извлекаем количество строк матрицы
				num_rows = std::stoi(line.substr(3));


				setIntevals = new Interval * [num_rows];

				// Считываем значения матрицы
				for (int i = 0; i < num_rows; i++)
				{

					try
					{
						getline(input, line);


						if (line.substr(0, 2) == ".e")
						{
							if (i < num_rows)
							{
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


					for (int cix = 0; cix < count_var; cix++)
					{
						char_array[cix] = line[cix];
					}

					char_array[count_var] = '\0';

					setIntevals[i] = new Interval(char_array);

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



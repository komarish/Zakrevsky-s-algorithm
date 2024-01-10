#include "BoolEquation.h"


#include <iostream>

#include <fstream>
#include <string>

#include <exception> 



void BoolEquation::PrintEquation()
{

	// ������� ��������� �������
	std::cout << "MATRIX" << std::endl;
	std::cout << "num_rows: " << num_rows << std::endl;
	std::cout << "count_var:" << count_var << std::endl;

	for (int ix = 0; ix < num_rows; ix++)
	{
		cout << (string)(*setIntevals[ix]) << endl;
	}

}



bool BoolEquation::isSolution(Interval Solution)
{
	if (!setIntevals)
	{
		return false;
	}

	for (int i = 0; i < num_rows; i++)
	{
		//���� ������� �� ������������ ���� �� ������ ���������,
		//���������� false
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

	if (input.is_open()) 
	{

		string line;
		int num_outputs;

		while (getline(input, line)) 
		{

			// ���������� ����������� � ������ ������
			if (line.empty() || line[0] == '#') continue;

			line.erase(line.find_last_not_of(" \n\r\t") + 1);

			// ��������� ���������� � ������, ������� � �������
			if (line.substr(0, 2) == ".i")
			{
				// ��������� ���������� ������
				count_var = std::stoi(line.substr(3));
			}
			else if (line.substr(0, 2) == ".o")
			{
				// ��������� ���������� �������
				num_outputs = std::stoi(line.substr(3));
			}
			else if (line.substr(0, 2) == ".p")
			{
				// ��������� ���������� ����� �������
				num_rows = std::stoi(line.substr(3));


				setIntevals = new Interval * [num_rows];

				// ��������� �������� �������
				for (int i = 0; i < num_rows; i++)
				{

					
					getline(input, line);


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
		cerr << "File not open." << file_name << endl;
	}

}


BoolEquation::~BoolEquation()
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
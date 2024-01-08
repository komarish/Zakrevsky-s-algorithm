// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "Algorithm.h"


#include <iostream>

#include <fstream>
#include <string>

#include <exception> 

using namespace std;

#include <vector>

void readFromPlaFile(string file_name)
{
	std::ifstream input(file_name);

	if (input.is_open()) {
		std::vector<std::vector<int>> matrix;
		std::string line;

		while (getline(input, line)) {
			// Пропускаем комментарии
			if (line[0] == '#') continue;

			line.erase(line.find_last_not_of(" \n\r\t") + 1);

			// Считываем информацию о входах, выходах и матрице
			if (line.substr(0, 2) == ".i") {
				// Извлекаем количество входов
				int num_inputs = std::stoi(line.substr(3));
				std::cout << "Количество входов: " << num_inputs << std::endl;
			}
			else if (line.substr(0, 2) == ".o") {
				// Извлекаем количество выходов
				int num_outputs = std::stoi(line.substr(3));
				std::cout << "Количество выходов: " << num_outputs << std::endl;
			}
			else if (line.substr(0, 2) == ".p") {
				// Извлекаем количество строк матрицы
				int num_rows = std::stoi(line.substr(3));
				std::cout << "Количество строк матрицы: " << num_rows << std::endl;

				// Расширяем матрицу до нужного размера
				matrix.resize(num_rows, std::vector<int>());

				// Считываем значения матрицы
				for (int i = 0; i < num_rows; ++i) {
					getline(input, line);
					line.erase(line.find_last_not_of(" \n\r\t") + 1);

					for (char c : line) {
						if (c != ' ') {
							if (c == '-')
							{
								matrix[i].push_back(2);
							}

							else
							{
								matrix[i].push_back(c - '0');
							}
							
						}
					}
				}
			}
		}

		// Выводим считанную матрицу
		std::cout << "Матрица:" << std::endl;
		for (const auto& row : matrix) {
			for (int val : row) {
				std::cout << val << " ";
			}
			std::cout << std::endl;
		}

		input.close();
	}
	else {
		std::cerr << "Не удалось открыть файл " << file_name << std::endl;
	}

}




void readFromPla(string file_name)
{
	std::ifstream input(file_name);

	if (input.is_open()) {

		std::string line;
		int num_rows;
		int num_inputs;
		int num_outputs;

		Interval** setIntevals;

		while (getline(input, line)) {
			// Пропускаем комментарии
			if (line[0] == '#') continue;

			line.erase(line.find_last_not_of(" \n\r\t") + 1);

			// Считываем информацию о входах, выходах и матрице
			if (line.substr(0, 2) == ".i") 
			{
				// Извлекаем количество входов
				num_inputs = std::stoi(line.substr(3));
				std::cout << "num_inputs: " << num_inputs << std::endl;
			}
			else if (line.substr(0, 2) == ".o") 
			{
				// Извлекаем количество выходов
				num_outputs = std::stoi(line.substr(3));
				std::cout << "num_outputs: " << num_outputs << std::endl;
			}
			else if (line.substr(0, 2) == ".p") 
			{
				// Извлекаем количество строк матрицы
				num_rows = std::stoi(line.substr(3));
				std::cout << "num_rows: " << num_rows << std::endl;

				// Расширяем матрицу до нужного размера
				//matrix.resize(num_rows, std::vector<int>());

				setIntevals = new Interval * [num_rows];

				// Считываем значения матрицы
				for (int i = 0; i < num_rows; i++) 
				{
					
					try
					{
						getline(input, line);


						if (line.substr(0, 2) == ".e")
						{
							cout << "iii = " << i << "   " << line << endl;
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
						cout << e.what()<<endl;
						exit(3);
					}

					line.erase(line.find_last_not_of(" \n\r\t") + 1);


					int length = line.length();

					char* char_array = new char[length + 1];

					// copying the contents of the 
					// string to char array 
					//strcpy_s(char_array, sizeof(char_array), line.c_str());

					for(int cix = 0; cix < num_inputs; cix++)
					{
						char_array[cix] = line[cix];
					}

					char_array[num_inputs] = '\0';
					
					setIntevals[i] = new Interval(char_array);

					cout << "i=" << i << endl;

					cout << (string)(*setIntevals[i]) << endl;

					delete[] char_array;
					
				}

				// Выводим считанную матрицу
				std::cout << "Matrix:" << std::endl;

				for (int ix = 0; ix < num_rows; ix++)
				{
					cout << (string)(*setIntevals[ix]) << endl;
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





/**/
int main()
{
	
	std::cout << "Hello World!\n";




	string path = "../DNF_Random_examples/dnfRnd_2.pla";
	

	string line;
	ifstream myfile(path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			cout << line << '\n';
		}
		myfile.close();

		cout << "\nreadFromPlaFile" << endl;

		//readFromPlaFile(path);
		readFromPla(path);
	}

	else cout << "Unable to open file";







	/*
   

    Algorithm* alg = new Algorithm();     
 

    Heuristics* IH;   

    //набор эвристик
    greedHeuristics gH;      
    simpleHeuristics  sH;

    //cout << "greedHeuristics do : ";
    //IH = &gH;
    //alg->setHeuristics(IH);
    //alg->findRoot();
    //cout << alg->getHeuristicsName() << endl;


    cout << "simpleHeuristics do : ";
    IH = &sH;
    alg->setHeuristics(IH);
    alg->findRoot();
    cout << alg->getHeuristicsName() << endl;

    //delete alg;
    //delete IH;

	 */

	/*
	char setII[10][100] = {
			"00--1-1-100",
			//"1-----1-100",
			//"10011-00000",
			"1-----1-101",
			"10011-00001",
			"--001-1----",
			"---1001-000",
			//"00000-1-100",
			"----------1",
			"--000000---",
			"1---1-0010-",
			"--0-----100",
			//"-----------",
			"1--11-1--00"
	};

	int sz = 10;
	//
	Interval** setIntevals = new Interval * [sz];

	for (int ix = 0; ix < sz; ix++)
	{
		setIntevals[ix] = new Interval(setII[ix]);
	}
	//setIntevals[ix] - хранится адрес объекта
	//*(setIntevals[ix]) - содержимое объекта
	//(string)(*setIntevals[ix]) - явное преобразование в строку
	for (int ix = 0; ix < sz; ix++)
	{
		cout << (string)(*setIntevals[ix]) << endl;
	}

	cout << "end" << endl;

	//int count_var = setIntevals[0]->length();
	//count_var = 
	//BBV rule_2(sz);
	//cout << rule_2 << endl;

	Component c = scanMatrixHorizontal(setIntevals, sz);
	c.Print();

	//c = scanMatrixVertical(setIntevals, sz);
	//c.Print();

	Interval** newDnf;

	newDnf = rule_5(c, setIntevals, sz);

	
	//for (int i = 0; i < sz; i++)
	//{
	//	if (newDnf[i])
	//	{
	//		cout << (newDnf[i]->rang()) << endl;
	//	}
	//	//cout << newDnf[i] << endl;
	//}
	

	c = Heur(setIntevals, sz);
	c.Print();

	*/

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

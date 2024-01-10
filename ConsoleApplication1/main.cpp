// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "Algorithm.h"


#include <iostream>
#include <fstream>
#include <string>
#include <exception> 
#include <vector>



using namespace std;

// Функция проверки pla-файла на корректность
// Функция возвращает false (файл не корректен), когда:
// - файл не удалось открыть
// - файл пуст
// - когда количество выходов не равно нулю (требования в данной задаче)
// - когда количество переменных не совпадает с объявленным в файле или оно равно нулю
// - когда количество строк не совпадает с объявленным в файле или оно равно нулю
// - когда символ в строке матрицы не является одним из трех: (-, 0, 1)

bool isPlaFileCorrect(const string& file_name)
{

	ifstream input(file_name);

	// Проверяем, что файл открылся
	if (!input.is_open())
	{
		cout << file_name << ": " << "Cannot open file.\n";
		return false;
	}


	//Проверяем, что файл не пуст
	if (input.peek() == EOF)
	{
		input.close();

		cout << "File is empty.\n";
		return false;
	}


	string line;

	int num_rows = 0; //кол-во строк (.p)
	int num_inputs = 0; //кол-во входов (.i)
	int num_outputs = 0; //кол-во выходов (.o)
	int count_var = 0; //кол-во переменных

	while (getline(input, line))
	{

		// Пропускаем комментарии и пустые строки
		if (line.empty() || line[0] == '#')
		{
			continue;
		}

		line.erase(line.find_last_not_of(" \n\r\t") + 1);

		// Считываем информацию о входах, выходах и матрице
		if (line.substr(0, 2) == ".i")
		{
			// Извлекаем количество входов
			count_var = std::stoi(line.substr(3));

			// Если количество переменных равно нулю
			if (!count_var)
			{
				input.close();

				cout << file_name << ": " << "Incorrect number of inputs.\n";
				return false;
			}


		}
		else if (line.substr(0, 2) == ".o")
		{
			// Извлекаем количество выходов
			num_outputs = std::stoi(line.substr(3));
			
			// Если количество выходов не равно нулю
			if (num_outputs)
			{
				input.close();

				cout << file_name << ": " << "Incorrect number of outputs (should be 0).\n";
				return false;
			}
		}
		else if (line.substr(0, 2) == ".p")
		{
			// Извлекаем количество строк матрицы
			num_rows = std::stoi(line.substr(3));

			// Если количество строк равно нулю
			if (!num_rows)
			{
				input.close();

				cout << file_name << ": " << "Incorrect number of rows.\n";
				return false;
			}


			// Считываем значения матрицы
			for (int i = 0; i < num_rows; i++)
			{

				
				getline(input, line);

				// Проверка, что строк не меньше,
				// чем указано в .p
				if (line.substr(0, 2) == ".e")
				{
					if (i < num_rows)
					{	
						input.close();

						cout << file_name << ": " << "Incorrect number of lines.\n";
						return false;
					}
				}
				

				line.erase(line.find_last_not_of(" \n\r\t") + 1);

				int lineCount = 0;

				for (char& c : line)
				{
					// Проверка символов строки
					if (c != '-' && c != '0' && c != '1')
					{
						input.close();
						
						cout << file_name << ": " << "Incorrect character.\n";
						return false;
					}
					
					lineCount++;
				}

				// Проверяем, что количество считанных символов
				// равно указанному количеству переменных
				if (lineCount != count_var)
				{
					input.close();
					
					cout << file_name << ": " << "Incorrect line or incorrect number of inputs.\n";
					return false;
				}

			}

		}
	}


	// Если в файле не указаны нужные параметры
	if (!count_var || !num_rows)
	{
		input.close();

		cout << file_name << ": " << "Incorrect PLA notation.\n";
		return false;
	}



	input.close();
	return true;

}


//В функции main демонстрируется работа алгоритма на примерах.
//В качестве примеров были использованы предложенные примеры (DNF_Random_examples)
//и несколько примеров, составленных самостоятельно.
//Директория "DNF_Random_examples/my_examples/" содержит два тривиальных примера my_pla_4.pla и my_pla_5.pla,
//которые нужны для демонстрации работы алгоритма, когда уравнение не имеет корней.

int main()
{
	//пути к файлам с примерами
	vector<string> examples_paths;

	//количество файлов в каждой из директорий
	int examples_count = 12,
		my_examples_count = 6;


	//Проверяем файлы и отбираем корректные файлы 

	cout << "Checking the correctness of files...\n";

	string examples_path = "../DNF_Random_examples/";

	for (int i = 1; i <= examples_count; i++)
	{
		string path = examples_path + "dnfRnd_" + to_string(i) + ".pla";

		//Если файл прошел проверку, добавляем путь к нему в список
		if (isPlaFileCorrect(path))
		{
			examples_paths.push_back(path);
		}
	}


	//Аналогичная проверка для второй директории

	string my_examples_path = "../DNF_Random_examples/my_examples/";

	for (int i = 1; i <= my_examples_count; i++)
	{
		string path = my_examples_path + "my_pla_" + to_string(i) + ".pla";

		if (isPlaFileCorrect(path))
		{
			examples_paths.push_back(path);
		}
	}

	// Перечисляем файлы, которые были отобраны

	cout << "\nThe following examples will be used:\n\n";

	for (auto p : examples_paths)
	{
		cout << p << endl;
	}

	//указатель на объект класса алгоритма
	Algorithm* alg = new Algorithm(); 

	//указатель на базовый класс эвристик
	Heuristics* IH; 

	//объекты классов эвристик
	greedHeuristics gH;
	simpleHeuristics sH;


	//Находим корень (или убеждаемся, что его не существует) для всех отобранных примеров.
	//Считываем матрицу из pla-файла и, если она не слишком большая (до 50 строк и 30 переменных),
	//выводим в консоль.
	//Сначала ищем корень с помощью greedHeuristics, затем с помощью simpleHeuristics.
	//Помимо найденного корня выводится количество итераций, потребовавшееся для нахождения корня. 
	//Итерации считались по количеству операций упрощения матрицы (метод Simplification).
	//Как правило, greedHeuristics демонстрирует результат хуже по сравнению с simpleHeuristics.
	//После нахождения корня идет проверка найденного решения на то, что это действительно корень.

	for (auto p : examples_paths)
	{
		//Выводим имя текущего файла

		cout << "\nCurrent example: " << p << "\n\n";


		BoolEquation equation;

		string root;

		//Считываем матрицу из файла
		equation.readFromPla(p);

		//Вывод матрицы
		if (equation.getCountVar() < 30 && equation.getNumRows() < 50)
		{
			equation.PrintEquation();
		}
		
		else
		{
			cout << "The matrix is too large, equation is not output.\n" << endl;
		}


		//Поиск корня с помощью greedHeuristics

		IH = &gH;
		alg->setHeuristics(IH);

		cout << "\nHeuristics Name: " << alg->getHeuristicsName() << endl;

		root = alg->findRoot(equation);


		cout << "Root: \n" << root << endl;
		cout << "Number of iterations: " << alg->getLastCountOfIterations() << endl;

		Interval greedSolution(root.c_str());

		bool answer = equation.isSolution(greedSolution);

		if (answer)
		{
			cout << "This vector is Solution.\n";
		}

		else
		{
			cout << "This vector is NOT Solution.\n";
		}

		//Поиск корня с помощью simpleHeuristics


		IH = &sH;
		alg->setHeuristics(IH);

		cout << "\nHeuristics Name: " << alg->getHeuristicsName() << endl;

		root = alg->findRoot(equation);


		cout << "Root: \n" << root << endl;
		cout << "Number of iterations: " << alg->getLastCountOfIterations() << endl;

		Interval simpleSolution(root.c_str());

		answer = equation.isSolution(simpleSolution);

		if (answer)
		{
			cout << "This vector is Solution.\n";
		}

		else
		{
			cout << "This vector is NOT Solution.\n";
		}


		cout << "\n\n";

	}


	delete alg;

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

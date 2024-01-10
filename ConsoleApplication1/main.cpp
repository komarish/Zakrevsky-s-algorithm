// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "Algorithm.h"


#include <iostream>

#include <fstream>
#include <string>

#include <exception> 

#include <vector>



using namespace std;


bool isPlaFileCorrect(const string& file_name)
{

	ifstream input(file_name);

	if (!input.is_open())
	{
		cout << file_name << ": " << "Cannot open file.\n";
		return false;
	}


	if (input.peek() == EOF)
	{
		input.close();

		cout << "File is empty.\n";
		return false;
	}


	string line;

	int num_rows = 0;
	int num_inputs = 0;
	int num_outputs = 0;
	int count_var = 0;

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
				// чем указано
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

					if (c != '-' && c != '0' && c != '1')
					{
						input.close();
						
						cout << file_name << ": " << "Incorrect character.\n";
						return false;
					}
					
					lineCount++;
				}

				if (lineCount != count_var)
				{
					input.close();
					
					cout << file_name << ": " << "Incorrect line or incorrect number of inputs.\n";
					return false;
				}

			}

		}
	}


	// Если вообще не встретили таких переменных
	if (!count_var || !num_rows)
	{
		input.close();

		cout << file_name << ": " << "Incorrect PLA notation.\n";
		return false;
	}




	input.close();
	return true;

}



/**/
int main()
{
	
	cout << "Checking the correctness of files...\n";


	vector<string> examples_paths, my_examples_paths;

	int examples_count = 12,
		my_examples_count = 6;


	string examples_path = "../DNF_Random_examples/";

	for (int i = 1; i <= examples_count; i++)
	{
		string path = examples_path + "dnfRnd_" + to_string(i) + ".pla";

		if (isPlaFileCorrect(path))
		{
			examples_paths.push_back(path);
		}
	}



	string my_examples_path = "../DNF_Random_examples/my_examples/";

	for (int i = 1; i <= my_examples_count; i++)
	{
		string path = my_examples_path + "my_pla_" + to_string(i) + ".pla";

		if (isPlaFileCorrect(path))
		{
			my_examples_paths.push_back(path);
		}
	}


	cout << "\nThe following examples will be used:\n\n";

	for (auto p : examples_paths)
	{
		cout << p << endl;
	}


	for (auto p : my_examples_paths)
	{
		cout << p << endl;
	}


	Algorithm* alg = new Algorithm();
	Heuristics* IH;

	greedHeuristics gH;
	simpleHeuristics sH;


	for (auto p : examples_paths)
	{
		cout << "\nCurrent example: " << p << "\n\n";


		BoolEquation equation;

		string root;

		equation.readFromPla(p);

		if (equation.getCountVar() < 30 && equation.getNumRows() < 50)
		{
			equation.PrintEquation();
		}
		
		else
		{
			cout << "The matrix is too large, equation is not output.\n" << endl;
		}

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






	//string pla_path = "../DNF_Random_examples/dnfRnd_10.pla";

	//string pla_path = "../DNF_Random_examples/my_examples/my_pla_1.pla";
	//string pla_path = "../DNF_Random_examples/my_examples/my_pla_6.pla";


	//cout << "isPlaFileCorrect: "<< isPlaFileCorrect(pla_path) << endl;


	/*

	BoolEquation equation;

	string root;

	equation.readFromPla(pla_path);
	equation.PrintEquation();


	Algorithm* alg = new Algorithm();


	Heuristics* IH;

	//набор эвристик
	greedHeuristics gH;
	simpleHeuristics sH;

	//cout << "greedHeuristics do : ";
	IH = &gH;
	alg->setHeuristics(IH);
	//alg->findRoot(equation);
	//cout << alg->getHeuristicsName() << endl;


	root = alg->findRoot(equation);

	cout << "Root (" << pla_path << "): \n" << root << endl;

	Interval Solution1(root.c_str());

	bool answer = equation.isSolution(Solution1);

	if (answer)
	{
		cout << "This vector is Solution.\n";
	}

	else
	{
		cout << "This vector is NOT Solution.\n";
	}






	//-----
	//cout << "simpleHeuristics do : ";
	IH = &sH;
	alg->setHeuristics(IH);
	//cout << alg->getHeuristicsName() << endl;
	//-----

	



	root = alg->findRoot(equation);

	cout << "Root (" << pla_path << "): \n" << root << endl;

	Interval Solution2(root.c_str());

	answer = equation.isSolution(Solution2);

	if (answer)
	{
		cout << "This vector is Solution.\n";
	}

	else
	{
		cout << "This vector is NOT Solution.\n";
	}
	
	
	//cout << alg->getHeuristicsName() << endl;
	*/
	


	/*
	char setII[10][100] = {
			"000-0-",
			"010-0-",
			"--010-",
			"-10--0",
			"0---10",
			"-1-0-0",
			"1-11--",
			"00--00",
			"1-1-1-",
			"1--010"
	};



	int sz = 10;

	Interval** setIntevals = new Interval * [sz];

	for (int ix = 0; ix < sz; ix++)
	{
		setIntevals[ix] = new Interval(setII[ix]);
	}


	for (int ix = 0; ix < sz; ix++)
	{
		cout << (string)(*setIntevals[ix]) << endl;
	}

	cout << "end" << endl;



	Interval Solution("1-00-1");

	bool sol;
	sol = isSolution(setIntevals, Solution, sz);
	cout << "isSolution: " << sol << endl;



	for (int ix = 0; ix < sz; ix++)
	{
		delete setIntevals[ix];
		setIntevals[ix] = nullptr;
	}

*/

	/*

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
	*/






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

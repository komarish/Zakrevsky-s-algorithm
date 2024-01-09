// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "Algorithm.h"


#include <iostream>

#include <fstream>
#include <string>

#include <exception> 

using namespace std;



/**/
int main()
{
	
	std::cout << "Hello World!\n";

	//string pla_path = "../DNF_Random_examples/dnfRnd_2.pla";

	string pla_path = "../DNF_Random_examples/my_examples/my_pla_1.pla";
	//string pla_path = "../DNF_Random_examples/my_examples/my_pla_4.pla";


	BoolEquation equation;

	equation.readFromPla(pla_path);
	equation.PrintEquation();


	Algorithm* alg = new Algorithm();


	Heuristics* IH;

	//набор эвристик
	greedHeuristics gH;
	simpleHeuristics sH;

	//cout << "greedHeuristics do : ";
	//IH = &gH;
	//alg->setHeuristics(IH);
	//alg->findRoot();
	//cout << alg->getHeuristicsName() << endl;


	//cout << "simpleHeuristics do : ";
	IH = &sH;
	alg->setHeuristics(IH);
	cout << alg->getHeuristicsName() << endl;

	

	string root;

	root = alg->findRoot(equation);

	cout << "Root (" << pla_path << "):" << root << endl;

	Interval Solution(root.c_str());

	bool answer = equation.isSolution(Solution);

	if (answer)
	{
		cout << "This root is Solution.\n";
	}

	else
	{
		cout << "This root is NOT Solution.\n";
	}
	

	//cout << alg->getHeuristicsName() << endl;

	


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

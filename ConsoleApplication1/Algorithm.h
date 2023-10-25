#pragma once

#include "Heuristics.h"

#include <iostream>

using namespace std;






struct Solution
{
	//root
	//ещё что-нибудь, статистика мб
};


class Algorithm
{
	Heuristics* heuristics;
	//указатель на матрицу и её строки, чтобы можно было "вычеркивать"

public:
	enum HeuristicsFlags
	{
		SIMPLE,
		GREED
	};

	void setHeuristics(Heuristics* h)
	{
		heuristics = h;	
	}

	string getHeuristicsName()
	{
		return heuristics->getName();
	}

	//здесь не должно быть void, потому что нам нужно как-то выбирать компоненту
	void checkHeuristicRules()
	{	
		heuristics->checkRules();
	}

	bool check1() 
	{
		cout << "check1\n";
		return true;
	}

	bool check2()
	{
		cout << "check2\n";
		return true;
	}

	bool check3()
	{
		cout << "check3\n";
		return true;
	}

	bool check4()
	{
		cout << "check4\n";
		return true;
	}

	bool check5()
	{
		cout << "check5\n";
		return true;
	}


	int checkAlgorithmRules()
	{
		if (check1())
		{
			return 1;
		}

		if (check2())
		{
			return 2;
		}

		if (check3())
		{
			return 3;
		}

		if (check4())
		{
			return 4;
		}

		if (check5())
		{
			return 5;
		}
	
		return 0;
	}

	void findRoot()
	{
		//обход дерева

		checkHeuristicRules();
	}


};


#pragma once

#include "Heuristics.h"
#include "BBV.h"
#include "Interval.h"
#include "NodeBoolTree.h"
#include "BoolEquation.h"
#include "Component.h"

#include <iostream>
#include <stack>          // std::stack

using namespace std;




//Класс, реализующий алгоритм поиска корня уравнения D=0

class Algorithm
{
	//выбранная эвристика
	Heuristics* heuristics; 

	//количество итераций, полученное при последнем вызове findRoot
	int lastCountOfIterations; 
	
	
public:

	Algorithm()
		: lastCountOfIterations(0)
	{
		heuristics = nullptr;
	}

	//возвращает последнее количество итераций
	int getLastCountOfIterations()
	{
		return 
			lastCountOfIterations;
	}

	//установка эвристики
	void setHeuristics(Heuristics* h)
	{
		heuristics = h;	
	}

	//возвращает название эвристики
	string getHeuristicsName()
	{
		return heuristics->getName();
	}

	//выбор компоненты с помощью эвристических правил
	Component checkHeuristicRules(NodeBoolTree*& node, int sz, int countVar)
	{	

		return
			heuristics->checkRules(node, sz, countVar);

	}

	//проверка правил алгоритма и выбор компоненты
	//возвращает объект класса Component (номер компоненты, значение компоненты)
	//если номер компоненты (нумерация идет с нуля) равен -1, значит, ни одно из правил не сработало
	//если номер компоненты равен -2, то выполнилось второго правило алгоритма (строка из неопределенных компонент)
	Component checkAlgorithmRules(NodeBoolTree*& node, int sz, int countVar)
	{
		Component component;

		// правила 1 и 2
		component = scanMatrixHorizontal(node, sz, countVar);

		if (component.n_comp == -1)
		{
			// правила 3 и 4
			component = scanMatrixVertical(node, sz, countVar);

		}


		return component;
		
	}

	//второе правило алгоритма
	Component rule_2(NodeBoolTree*& node, int sz, int countVar);

	//первое правило алгоритма
	Component rule_1(NodeBoolTree*& node, int sz, int countVar);

	//анализ строк матрицы (правила 1-2)
	Component scanMatrixHorizontal(NodeBoolTree*& node, int sz, int countVar);

	//анализ столбцов матрицы (правила 3-4)
	Component scanMatrixVertical(NodeBoolTree*& node, int sz, int countVar);

	//удаление строк (правило 5)
	void Simplification(Component component, NodeBoolTree*& node, int sz);

	//поиск корня, использует функцию TreeTraversal
	string findRoot(BoolEquation& equation);

	//поиск решения; если корня не существует, возвращает вектор, все компоненты которого не определены
	string TreeTraversal(Interval** setIntevals, size_t size, unsigned countVar);

};


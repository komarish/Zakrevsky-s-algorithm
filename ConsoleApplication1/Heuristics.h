#pragma once

#include "BBV.h"
#include "Interval.h"
#include "NodeBoolTree.h"
#include "BoolEquation.h"
#include "Component.h"


#include <iostream>

using namespace std;



//Класс-интерфейс эвристики
class Heuristics
{
public:
	virtual Component checkRules(NodeBoolTree*& node, int sz, int countVar) = 0; //выбор компоненты
    virtual string getName() = 0; //вовращает название эвристики
};

//greedHeuristics
//В ситуации, когда ни одно из правил алгоритма не выполняется, руководствуемся следующими правилами:
//1. Выбор строки.
//Выбираем строку с минимальным рангом .
//2. Выбор компоненты.
//Рассматриваем определенные компоненты выбранной строки, подсчитываем, 
//сколько нулей и единиц в каждом столбце и выбираем ту компоненту, 
//в столбце которой наибольшее количество ячеек с одинаковыми значениями.

class greedHeuristics : public Heuristics 
{       
public:

	string getName()
	{
		return "greedHeuristics";
	}

	int findMinRangRowIndex(NodeBoolTree*& node, int sz, int countVar);

	Component checkRules(NodeBoolTree*& node, int sz, int countVar);
};




//simpleHeuristics
//В ситуации, когда ни одно из правил алгоритма не выполняется, 
//выбираем наиболее определенный столбец и присваиваем компоненте значение,
//противоположное значению большинства компонент в данном столбце.

class simpleHeuristics : public Heuristics 
{     
public:

	string getName()
	{
		return "simpleHeuristics";
	}
	
	Component checkRules(NodeBoolTree*& node, int sz, int countVar);
};
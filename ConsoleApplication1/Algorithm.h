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





class Algorithm
{
	Heuristics* heuristics;
	
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







void Simplification(Component component, NodeBoolTree*& node, int sz)
{

	
	int row_value;

	for(int i = 0; i < sz; i++)
	{
		
		
		if (!node->rowsToBeConsidered[i])
		{
			continue;
		}
		
		
		row_value = node->dnf[i]->getValue(component.n_comp) - '0';

		//строки, которые "удаляются" из матрицы
		if (row_value == !component.val_comp)
		{
			node->rowsToBeConsidered[i] = 0;
			node->count--;
		}

	
	}
	
}



	Component checkHeuristicRules(NodeBoolTree*& node, int sz, int countVar)
	{	

		return
			heuristics->checkRules(node, sz, countVar);

	}

	


	Component checkAlgorithmRules(NodeBoolTree*& node, int sz, int countVar)
	{
		Component component;

		//правила 1 и 2
		component = scanMatrixHorizontal(node, sz, countVar);

		if (component.n_comp == -1)
		{
			//правила 3 и 4
			component = scanMatrixVertical(node, sz, countVar);

		}


		return component;
		
	}



	// -2 - корня нет
	// -1 - ничего не найдено 
	Component rule_2(NodeBoolTree*& node, int sz, int countVar)
	{

		Component component;


		int i, j;

		//идём по строкам
		for (i = 0; i < sz; i++)
		{
			
			if (!node->rowsToBeConsidered[i])
			{
				continue;
			}
			
			//и столбцам
			for (j = 0; j < countVar; j++)
			{
				//рассматриваем только те стобцы, которые должны
				if (node->varsToBeConsidered[j])
				{
					//если хоть одна компонента не помечена черточкой,
					//т.е. dnc[j] != 0, то выходим из цикла 
					if (node->dnf[i]->getDnc()[j] == 0)
					{
						break;
					}
				}		
			}

			//если вся строка целиком состоит из черточек
			if (j == countVar)
			{
				component.n_comp = -2;

				return component;
			}

		}

		return component;
	}


	Component rule_1(NodeBoolTree*& node, int sz, int countVar)
	{

		Component component;

		for (int i = 0; i < sz; i++)
		{
			
			if (!node->rowsToBeConsidered[i])
			{
				continue;
			}
			
			Interval tmpI;

			BBV tmpVec;
			tmpVec = node->dnf[i]->getVec();

			BBV nodeDnc;
			nodeDnc = node->dnf[i]->getDnc();

			BBV tmpDnc = node->varsToBeConsidered;

			tmpDnc = ~tmpDnc;
			tmpDnc = tmpDnc | nodeDnc;

			tmpI.setVec(tmpVec);
			tmpI.setDnc(tmpDnc);

			
			//если только одна компонента определена
			if (tmpI.rang() == 1)
			{
				BBV dnc;
				dnc = node->dnf[i]->getDnc();

				//нужно проверить, в рассматриваемом ли она нами столбце
				//и, если да, посмотреть её значение
				for (int j = 0; j < countVar; j++)
				{

					if (node->varsToBeConsidered[j])
					{
						if (!dnc[j]) //компонента определена
						{

							component.n_comp = j;
							component.val_comp = !(node->dnf[i]->getVec()[j]);

							return component;

						}
					}
					
				}

			}

		}

		return component;
	}



	Component scanMatrixHorizontal(NodeBoolTree*& node, int sz, int countVar)
	{
		Component r2 = rule_2(node, sz, countVar);

		if (r2.n_comp == -2)
		{
			return r2;
		}

		else
		{
			return rule_1(node, sz, countVar);
		}

	}




	Component scanMatrixVertical(NodeBoolTree*& node, int sz, int countVar)
	{

		Component component;

		bool isNotDefined; //rule 3

		bool isOrth0, isOrth1; //rule 4

		int notDefinedIdx = -1;

		for (int j = 0; j < countVar; j++)
		{
			
			if (node->varsToBeConsidered[j])
			{

				isNotDefined = 1;
				isOrth0 = 1;
				isOrth1 = 1;


				for (int i = 0; i < sz; i++)
				{


					if (!node->rowsToBeConsidered[i])
					{
						continue;
					}


					BBV vec, dnc;

					vec = node->dnf[i]->getVec();
					dnc = node->dnf[i]->getDnc();

					if (!dnc[j])
					{
						isNotDefined = 0;

						if (vec[j] == 0)
						{
							isOrth0 = 0;
						}

						if (vec[j] == 1)
						{
							isOrth1 = 0;
						}

					}

				}

				if (isNotDefined)
				{
					isOrth0 = 0;
					isOrth1 = 0;
				}


				if (isOrth0)
				{
					component.n_comp = j;
					component.val_comp = 0;

					return component;
				}

				if (isOrth1)
				{
					component.n_comp = j;
					component.val_comp = 1;

					return component;
				}


				if (isNotDefined)
				{
					notDefinedIdx = j;
				}
			}

		}



		//тогда, получается, берем последний неопред. столбец
		if (notDefinedIdx != -1)
		{
			component.n_comp = notDefinedIdx;
			
			component.val_comp = 0; //может быть любое, присвоили 0
		}


		return component;

	}





	string findRoot(BoolEquation& equation)
	{

		int sz = equation.getNumRows();
		int countVar = equation.getCountVar();


		Interval** setIntevals = equation.getSetIntevals();

		Interval** setTest = new Interval * [sz];

		for (int ix = 0; ix < sz; ix++)
		{
			setTest[ix] = setIntevals[ix];
		}


		string root;


		root = TreeTraversal(setTest, sz, countVar);

		for (int ix = 0; ix < sz; ix++)
		{
			setTest[ix] = nullptr;
		}

		return root;
	}



	//множество интервалов, длина матрицы, ширина матрицы (кол-во переменных)
	string TreeTraversal(Interval** setIntevals, size_t size, unsigned countVar)
	{


		std::stack<NodeBoolTree*> mystack;

		bool continueTravesal = true;


		NodeBoolTree* currentNode = nullptr;

		//номер текущей переменной
		int posA = -1;


		BBV varsToBeConsidered(countVar);
		varsToBeConsidered = ~varsToBeConsidered;


		BBV rowsToBeConsidered(size);
		rowsToBeConsidered = ~rowsToBeConsidered;



		//нулевой булев вектор
		BBV Nol(countVar);


		//не нулевой рабочий вектор. Вектор в которм все компоненты принимают значение 1.
		BBV NotNol(countVar);
		NotNol = ~Nol;


		//рабочий интервал
		Interval tmpI(countVar);
		tmpI.setDnc(NotNol);


		
		//Создаем начальный узел дерева обхода
		//Инициализируем поле dnf(создаем множество интервалов для данного узла). 
		//В начальный момент времени это исходное множество.


		currentNode = new NodeBoolTree(countVar, size); 


		if (!currentNode->dnf)
		{
			//allocation failed
			cout << "allocation failed" << endl;
		}

		//Заполнение множества интервалов адресами интервалов исходного множества 
		for (int ix = 0; ix < size; ix++) 
		{
			//просто копируем адреса, из setIntevals[ix] в currentNode->dnf[ix].
			currentNode->dnf[ix] = setIntevals[ix];		
		}

		//Изначально задаем для полей  fixVal_0 и fixVal_1 значение false, 
		//что  будет говорить о том, что для данного узла еще не выполнялся анализ, 
		//не выбиралась переменная обхода и для нее не фиксировалось значение.
		currentNode->fixVal_0 = false;
		currentNode->fixVal_1 = false;

		//Инициализация интервалом tmpI в котором все переменные принимают значение '-'. 
		//По мере фиксирования значений в этом поле отмечаются советующие значения
		// !
		currentNode->Solution = tmpI;
		currentNode->varsToBeConsidered = varsToBeConsidered;
		currentNode->rowsToBeConsidered = rowsToBeConsidered;

		Component component;

		while (continueTravesal)
		{
		

			//Это условие, которое указвает, на то, что для данного узла не проводился анализ 
			if (currentNode && (!currentNode->fixVal_0 && !currentNode->fixVal_1)) 
			{
				//Здесь, необходимо анализировать текущее множество интервалов на предмет поиска 
				// переменной ветвления и фиксации значения для выбранной переменной ветвления.


				//Здесь, условие  - если рассматриваемы узел, в котором зафиксирована последняя переменная
				//(так как мы  последовательно перебираем переменные интервала) 
				//если это условие выполняется то, текущий узел нам не нужен, так как мы уже все вершины рассмотрели. 
				//Его удаляем, а из стека достаем новый.   


				if (currentNode->varsToBeConsidered == Nol)
				{
					

					delete currentNode;

					currentNode = nullptr;

					//Если стек пуст, то прекращаем обход
					if (mystack.empty())
					{
						continueTravesal = false;

						cout << "Root DOES NOT exist. Return empty vector.\n";
						return (string)tmpI;
					}

					//иначе достаем элемент из стека
					else
					{
						
						//записали адрес следующего элемента
						currentNode = mystack.top();


						//удаление из стека.
						mystack.pop();

					}
						
				}

				// Здесь фиксируем переменную обхода
				else
				{
					
					//ВЫБОР КОМПОНЕНТЫ И ЕЁ ЗНАЧЕНИЯ ПО АЛГОРИТМУ

					component = checkAlgorithmRules(currentNode, size, countVar);

					//ЕСЛИ СРАБОТАЛО ВТОРОЕ ПРАВИЛО, ТО
					//ВЫВОДИМ "КОРНЯ НЕТ" ПРЕКРАЩАЕМ ОБХОД

					if (component.n_comp == -2)
					{
						cout << "Root DOES NOT exist. Return empty vector.\n";
						return (string)tmpI;
					}


					//заносим в стек, если выбрали значения по эвристике
					if (component.n_comp == -1)
					{
						component = checkHeuristicRules(currentNode, size, countVar);

						NodeBoolTree* newNode = new NodeBoolTree(countVar, size);

						newNode->var = component.n_comp;

						if (component.val_comp == 0)
						{
							newNode->fixVal_1 = true;
						}

						if (component.val_comp == 1)
						{ 
							newNode->fixVal_0 = true;
						}


						//ЗДЕСЬ ДНФ ПРОСТО ПРИСВАИВАЕМ,
						//ПОТОМУ ЧТО У АЛЬТЕРНАТИВНОГО УЗЛА ОНА ТАКАЯ ЖЕ
						newNode->dnf = currentNode->dnf;
						newNode->count = currentNode->count;
						newNode->varsToBeConsidered = currentNode->varsToBeConsidered;
						newNode->rowsToBeConsidered = currentNode->rowsToBeConsidered;
						newNode->Solution = currentNode->Solution;


						//Заносим этот узел в стек
						mystack.push(newNode);

					}


					posA = component.n_comp;
					currentNode->var = posA;



					//ФИКСИРУЕМ ЗНАЧЕНИЯ В УЗЛЕ
					//
					//Напомню, что сюда попадаем в случае когда для узла не проводили анализа 

					if (component.val_comp == 0)
					{
						currentNode->fixVal_0 = true;
					}

					if (component.val_comp == 1)
					{
						currentNode->fixVal_1 = true;
					}

					
				}
			}

			//Сюда мы попадаем, в случае, когда одно из значений, у рассматриваемого узла, зафиксировано, а следовательно, 
			//нужно выполнить сокращения множества интервалов относительно выбранной переменной.
			//здесь, также, можно выполнять анализ получившегося множества. 
			//Если найдено решение, то обход дерева прекращается.
			//В нашем примере мы не анализируем, а просто имитируем сокращение, 
			//но помним, что после сокращения узел обновился, а следовательно,
			//сбрасываем все ранее зафиксированные значения. Теперь узел готов для дальнейшего анализа.
			else
			{
				

				

				// относительно текущей фиксации значений

				posA = currentNode->var;
				

				//СОЗДАНИЕ НОВОЙ ДНФ, МАСКИ И ОБНОВЛЕНИЕ УЗЛА


				currentNode->varsToBeConsidered[posA] = 0;


				//скользкий момент!
				
				int comp_value;

				if (currentNode->fixVal_0)
				{
					comp_value = 0;
					currentNode->Solution.setValue('0', posA);
				}

				else
				{
					comp_value = 1;
					currentNode->Solution.setValue('1', posA);
				}
				 

				Component component(currentNode->var, comp_value);

				Simplification(component, currentNode, size);

				/*
				
				cout << "Simplification " << endl;

				for (int i = 0; i < size; i++)
				{

					if (!currentNode->rowsToBeConsidered[i])
					{
						cout << "+" << endl;
					}

					else
					{
						cout << (string)*currentNode->dnf[i] << endl;
					}
				}


				cout << (string)currentNode->Solution << endl;

				*/


				//ЕСЛИ ДНФ ОКАЗАЛАСЬ ПУСТА, ТО НАЙДЕН КОРЕНЬ
				//ВЫВОДИМ "НАШЛИ КОРЕНЬ", САМ КОРЕНЬ И ПРЕКРАЩАЕМ ОБХОД
				//cout << "Solution:" << endl;

				

				if (currentNode->count == 0)
				{
					return (string)currentNode->Solution;
				}



				//Если нужно, то сбрасываем зафиксированные значения, в противном случае останавливаем обход.
				currentNode->fixVal_0 = false;
				currentNode->fixVal_1 = false;


			}
		}
		
	}

};


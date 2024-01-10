#include "Algorithm.h"


void Algorithm::Simplification(Component component, NodeBoolTree*& node, int sz)
{

	int row_value;

	for (int i = 0; i < sz; i++)
	{

		//пропускаем строки, которые помечены как удаленные
		if (!node->rowsToBeConsidered[i])
		{
			continue;
		}


		row_value = node->dnf[i]->getValue(component.n_comp) - '0';

		//помечаем нулем строки, которые "удаляются" из матрицы
		if (row_value == !component.val_comp)
		{
			node->rowsToBeConsidered[i] = 0;
			node->count--;
		}
	}
}



Component Algorithm::rule_2(NodeBoolTree*& node, int sz, int countVar)
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


Component Algorithm::rule_1(NodeBoolTree*& node, int sz, int countVar)
{

	Component component;

	for (int i = 0; i < sz; i++)
	{

		//рассматриваем только оставшиеся строки
		if (!node->rowsToBeConsidered[i])
		{
			continue;
		}


		//на основе вектора varsToBeConsidered (маска по столбцам) 
		//создаем вектор, ранг которого будем вычислять

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




Component Algorithm::scanMatrixHorizontal(NodeBoolTree*& node, int sz, int countVar)
{
	Component r2 = rule_2(node, sz, countVar);

	//если не выполнилось второе правило (более критично),
	//то проверяем правило 1
	if (r2.n_comp == -2)
	{
		return r2;
	}

	else
	{
		return 
			rule_1(node, sz, countVar);
	}

}




Component Algorithm::scanMatrixVertical(NodeBoolTree*& node, int sz, int countVar)
{

	Component component;

	bool isNotDefined; //rule 3
	int notDefinedIdx = -1;

	bool isOrth0, isOrth1; //rule 4

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
					//если компонента определена,
					//то флаг isNotDefined сразу принимает значение false
					isNotDefined = 0;


					//если значение равно 0,
					//то флаг isOrth0 (столбец состоит полностью из 1) 
					//сразу принимает значение false
					if (vec[j] == 0)
					{
						isOrth0 = 0;
					}

					//если значение равно 1,
					//то флаг isOrth1 (столбец состоит полностью из 0) 
					//сразу принимает значение false
					if (vec[j] == 1)
					{
						isOrth1 = 0;
					}

				}

			}

			//поскольку для сокращения матрицы
			//приоритетнее найти столбцы, состоящие из 0 или 1,
			//мы не возвращаем компоненту, а запоминаем индекс столбца
			//(тогда берется последний пустой столбец)
			if (isNotDefined)
			{
				isOrth0 = 0;
				isOrth1 = 0;

				notDefinedIdx = j;
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

		}

	}



	//если стобцов с 0 или 1 не нашлось, то
	//возвращаем индекс столбца из неопределенных компонент
	if (notDefinedIdx != -1)
	{
		component.n_comp = notDefinedIdx;

		//согласно правилу, значение может быть любое, присвоили 0
		component.val_comp = 0; 
	}


	return component;

}



string Algorithm::findRoot(BoolEquation& equation)
{

	int sz = equation.getNumRows(); //значение количества строк
	int countVar = equation.getCountVar(); //количество переменных


	Interval** setIntevals = equation.getSetIntevals(); //множество интервалов

	//копируем адреса

	Interval** setTest = new Interval * [sz];

	for (int ix = 0; ix < sz; ix++)
	{
		setTest[ix] = setIntevals[ix];
	}


	string root;

	//ищем корень
	root = TreeTraversal(setTest, sz, countVar);

	for (int ix = 0; ix < sz; ix++)
	{
		setTest[ix] = nullptr;
	}

	return root;
}




//множество интервалов, длина матрицы, ширина матрицы (кол-во переменных)
string Algorithm::TreeTraversal(Interval** setIntevals, size_t size, unsigned countVar)
{

	//стек узлов с альтернативными значениями
	std::stack<NodeBoolTree*> mystack; 


	//текущий узел
	NodeBoolTree* currentNode = nullptr;

	//номер текущей переменной
	int posA = -1;

	//маска на столбцы, изначально все столбцы помечены 1
	BBV varsToBeConsidered(countVar);
	varsToBeConsidered = ~varsToBeConsidered;

	//маска на строки, изначально все строки помечены 1
	BBV rowsToBeConsidered(size);
	rowsToBeConsidered = ~rowsToBeConsidered;

	//нулевой булев вектор
	BBV Nol(countVar);


	//не нулевой рабочий вектор (все компоненты равны 1)
	BBV NotNol(countVar);
	NotNol = ~Nol;


	//рабочий интервал
	Interval tmpI(countVar);
	tmpI.setDnc(NotNol);



	//Создаем начальный узел дерева обхода
	//Инициализируем поле dnf(создаем множество интервалов для данного узла). 
	//В начальный момент времени это исходное множество.

	currentNode = new NodeBoolTree(countVar, size);


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
	currentNode->Solution = tmpI;

	//Инициализация масок на строки и столбцы
	currentNode->varsToBeConsidered = varsToBeConsidered;
	currentNode->rowsToBeConsidered = rowsToBeConsidered;

	//компонента
	Component component;

	//счётчик итераций
	lastCountOfIterations = 0;


	while (true)
	{

		//Если для данного узла не проводился анализ 
		if (currentNode && (!currentNode->fixVal_0 && !currentNode->fixVal_1))
		{
			
			//Если мы рассмотели все переменные, то нужно проверить,
			//есть ли альтернативные узлы в стеке.
			if (currentNode->varsToBeConsidered == Nol)
			{


				//Если стек пуст, то прекращаем обход, 
				//поскольку корня не существует
				if (mystack.empty())
				{
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

			// Здесь фиксируем переменную обхода и её значение
			else
			{
			
				//проверяем по правилам алгоритма
				component = checkAlgorithmRules(currentNode, size, countVar);

				//Если сработало второе правило, то прекращаем обход, 
				//поскольку корня не существует

				if (component.n_comp == -2)
				{
					cout << "Root DOES NOT exist. Return empty vector.\n";
					return (string)tmpI;
				}


				//Если правила алгоритма не выполняются, проверяем правила эвристики
				if (component.n_comp == -1)
				{
					component = checkHeuristicRules(currentNode, size, countVar);

					//создаем альтернативный узел и заносим в стек, если выбрали значение по эвристике

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

					//ДНФ альтернативного узла и решение такие же
					newNode->dnf = currentNode->dnf; 
					newNode->count = currentNode->count;
					newNode->varsToBeConsidered = currentNode->varsToBeConsidered;
					newNode->rowsToBeConsidered = currentNode->rowsToBeConsidered;
					newNode->Solution = currentNode->Solution;


					//Заносим этот узел в стек
					mystack.push(newNode);

				}


				//меняем номер текущей позиции
				posA = component.n_comp;
				currentNode->var = posA;


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

		//Когда значение текущей компоненты зафиксировано, выполняем сокращение множества интервалов
		//и обновляем узел и все его характеристики.
		//Если множество интервалов после сокращения оказалось пустым, это признак того, что корень найден.

		else
		{

			//относительно текущей фиксации значений
			posA = currentNode->var;

			//помечаем компоненту как рассмотренную
			currentNode->varsToBeConsidered[posA] = 0;


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

			//выполняем сокращение
			Simplification(component, currentNode, size);

			//отладочный вывод промежуточных состояний матрицы и текущего решения

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


			//если матрица пуста, возвращаем корень

			if (currentNode->count == 0)
			{
				return (string)currentNode->Solution;
			}



			//иначе сбрасываем значения и 
			//увеличиваем счетчик итераций

			currentNode->fixVal_0 = false;
			currentNode->fixVal_1 = false;

			lastCountOfIterations++;

		}
	}

}
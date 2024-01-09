#pragma once

#include "BBV.h"
#include "Interval.h"
#include "NodeBoolTree.h"
#include "BoolEquation.h"
#include "Component.h"


#include <iostream>

using namespace std;




class Heuristics
{
public:
	virtual Component checkRules(NodeBoolTree*& node, int sz, int countVar) = 0;
    virtual string getName() = 0;
};

class greedHeuristics : public Heuristics 
{       
public:

	int findMinRangRowIndex(NodeBoolTree*& node, int sz, int countVar)
	{
		int min_i = 0; //индекс мин. опред. строки

		int min_rang = countVar + 1; 


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

			int rang = tmpI.rang();

			if (tmpI.rang() < min_rang)
			{
				min_rang = rang;
				min_i = i;
			}


		}


		return min_i;
	}

    Component checkRules(NodeBoolTree*& node, int sz, int countVar)
    {
        //cout << "greedHeuristics - checkRules\n";

		Component component;

		int min_i = findMinRangRowIndex(node, sz, countVar);

		int max_num = 0, tmp_max = 0,
			tmp_0, tmp_1; //кол-во единиц и нулей

		//int value = 0, tmp_value = 0, vect_value, index = 0;
		int value = 0, tmp_value = 0, vect_value, index;

		for (int j = 0; j < countVar; j++)
		{
			//определенные компоненты
			if (node->varsToBeConsidered[j] && !node->dnf[min_i]->getDnc()[j])
			{
				tmp_0 = 0;
				tmp_1 = 0;

				for (int i = 0; i < sz; i++)
				{
					if (!node->rowsToBeConsidered[i])
					{
						continue;
					}


					vect_value = node->dnf[i]->getValue(j) - '0';

					if (vect_value == 0)
					{
						tmp_0++;
					}

					if (vect_value == 1)
					{
						tmp_1++;
					}

				}


				if (tmp_0 > tmp_1)
				{
					tmp_max = tmp_0;

					tmp_value = 0;


				}
				else
				{
					tmp_max = tmp_1;

					tmp_value = 1;


				}


				if (tmp_max > max_num)
				{
					max_num = tmp_max;

					index = j;
					value = tmp_value;



				}

				
			}
		}
		


		component.n_comp = index;
		component.val_comp = value;


		return component;
    }

    string getName()
    {
        return "greedHeuristics";
    }
};

class simpleHeuristics : public Heuristics 
{     
public:

    Component checkRules(NodeBoolTree*& node, int sz, int countVar)
    {
        //cout << "simpleHeuristics - checkRules\n";
	
	
		Component component;
	
		int max_j = 0; //индекс макс. опр. столбца

		int max_num_0 = 0, max_num_1 = 0,
			tmp_0, tmp_1; //кол-во единиц и нулей

		int value;



		for (int j = 0; j < countVar; j++)
		{
			if (node->varsToBeConsidered[j])
			{
			
				tmp_0 = 0;
				tmp_1 = 0;

				for (int i = 0; i < sz; i++)
				{
			
					if (!node->rowsToBeConsidered[i])
					{
						continue;
					}

					
					value = node->dnf[i]->getValue(j) - '0';

					if (value == 0)
					{
						tmp_0++;
					}

					if (value == 1)
					{
						tmp_1++;
					}

				}

				if (tmp_0 + tmp_1 > max_num_0 + max_num_1)
				{
					max_j = j;

					max_num_0 = tmp_0;
					max_num_1 = tmp_1;

				}
			}

		}
	

		component.n_comp = max_j;

		if (max_num_0 > max_num_1)
		{
			component.val_comp = 1;
		}

		else
		{
			component.val_comp = 0;
		}

		return component;


    }

    string getName()
    {
        return "simpleHeuristics";
    }
};
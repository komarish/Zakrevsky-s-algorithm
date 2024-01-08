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
    Component checkRules(NodeBoolTree*& node, int sz, int countVar)
    {
        cout << "greedHeuristics - checkRules\\n";

		Component component;

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
        cout << "simpleHeuristics - checkRules\n";
	
	
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
			
					if (!node->dnf[i])
					{
						//break;
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
	
		/*
		cout << "max_j = " << max_j << endl;
		cout << "max_num_0 = " << max_num_0 << endl;
		cout << "max_num_1 = " << max_num_1 << endl;
		*/

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
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

		//������, ������� "���������" �� �������
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

		//������� 1 � 2
		component = scanMatrixHorizontal(node, sz, countVar);

		if (component.n_comp == -1)
		{
			//������� 3 � 4
			component = scanMatrixVertical(node, sz, countVar);

		}


		return component;
		
	}



	// -2 - ����� ���
	// -1 - ������ �� ������� 
	Component rule_2(NodeBoolTree*& node, int sz, int countVar)
	{

		Component component;


		int i, j;

		//��� �� �������
		for (i = 0; i < sz; i++)
		{
			
			if (!node->rowsToBeConsidered[i])
			{
				continue;
			}
			
			//� ��������
			for (j = 0; j < countVar; j++)
			{
				//������������� ������ �� ������, ������� ������
				if (node->varsToBeConsidered[j])
				{
					//���� ���� ���� ���������� �� �������� ���������,
					//�.�. dnc[j] != 0, �� ������� �� ����� 
					if (node->dnf[i]->getDnc()[j] == 0)
					{
						break;
					}
				}		
			}

			//���� ��� ������ ������� ������� �� ��������
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

			
			//���� ������ ���� ���������� ����������
			if (tmpI.rang() == 1)
			{
				BBV dnc;
				dnc = node->dnf[i]->getDnc();

				//����� ���������, � ��������������� �� ��� ���� �������
				//�, ���� ��, ���������� � ��������
				for (int j = 0; j < countVar; j++)
				{

					if (node->varsToBeConsidered[j])
					{
						if (!dnc[j]) //���������� ����������
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



		//�����, ����������, ����� ��������� �������. �������
		if (notDefinedIdx != -1)
		{
			component.n_comp = notDefinedIdx;
			
			component.val_comp = 0; //����� ���� �����, ��������� 0
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



	//��������� ����������, ����� �������, ������ ������� (���-�� ����������)
	string TreeTraversal(Interval** setIntevals, size_t size, unsigned countVar)
	{


		std::stack<NodeBoolTree*> mystack;

		bool continueTravesal = true;


		NodeBoolTree* currentNode = nullptr;

		//����� ������� ����������
		int posA = -1;


		BBV varsToBeConsidered(countVar);
		varsToBeConsidered = ~varsToBeConsidered;


		BBV rowsToBeConsidered(size);
		rowsToBeConsidered = ~rowsToBeConsidered;



		//������� ����� ������
		BBV Nol(countVar);


		//�� ������� ������� ������. ������ � ������ ��� ���������� ��������� �������� 1.
		BBV NotNol(countVar);
		NotNol = ~Nol;


		//������� ��������
		Interval tmpI(countVar);
		tmpI.setDnc(NotNol);


		
		//������� ��������� ���� ������ ������
		//�������������� ���� dnf(������� ��������� ���������� ��� ������� ����). 
		//� ��������� ������ ������� ��� �������� ���������.


		currentNode = new NodeBoolTree(countVar, size); 


		if (!currentNode->dnf)
		{
			//allocation failed
			cout << "allocation failed" << endl;
		}

		//���������� ��������� ���������� �������� ���������� ��������� ��������� 
		for (int ix = 0; ix < size; ix++) 
		{
			//������ �������� ������, �� setIntevals[ix] � currentNode->dnf[ix].
			currentNode->dnf[ix] = setIntevals[ix];		
		}

		//���������� ������ ��� �����  fixVal_0 � fixVal_1 �������� false, 
		//���  ����� �������� � ���, ��� ��� ������� ���� ��� �� ���������� ������, 
		//�� ���������� ���������� ������ � ��� ��� �� ������������� ��������.
		currentNode->fixVal_0 = false;
		currentNode->fixVal_1 = false;

		//������������� ���������� tmpI � ������� ��� ���������� ��������� �������� '-'. 
		//�� ���� ������������ �������� � ���� ���� ���������� ���������� ��������
		// !
		currentNode->Solution = tmpI;
		currentNode->varsToBeConsidered = varsToBeConsidered;
		currentNode->rowsToBeConsidered = rowsToBeConsidered;

		Component component;

		while (continueTravesal)
		{
		

			//��� �������, ������� ��������, �� ��, ��� ��� ������� ���� �� ���������� ������ 
			if (currentNode && (!currentNode->fixVal_0 && !currentNode->fixVal_1)) 
			{
				//�����, ���������� ������������� ������� ��������� ���������� �� ������� ������ 
				// ���������� ��������� � �������� �������� ��� ��������� ���������� ���������.


				//�����, �������  - ���� �������������� ����, � ������� ������������� ��������� ����������
				//(��� ��� ��  ��������������� ���������� ���������� ���������) 
				//���� ��� ������� ����������� ��, ������� ���� ��� �� �����, ��� ��� �� ��� ��� ������� �����������. 
				//��� �������, � �� ����� ������� �����.   


				if (currentNode->varsToBeConsidered == Nol)
				{
					

					delete currentNode;

					currentNode = nullptr;

					//���� ���� ����, �� ���������� �����
					if (mystack.empty())
					{
						continueTravesal = false;

						cout << "Root DOES NOT exist. Return empty vector.\n";
						return (string)tmpI;
					}

					//����� ������� ������� �� �����
					else
					{
						
						//�������� ����� ���������� ��������
						currentNode = mystack.top();


						//�������� �� �����.
						mystack.pop();

					}
						
				}

				// ����� ��������� ���������� ������
				else
				{
					
					//����� ���������� � Ũ �������� �� ���������

					component = checkAlgorithmRules(currentNode, size, countVar);

					//���� ��������� ������ �������, ��
					//������� "����� ���" ���������� �����

					if (component.n_comp == -2)
					{
						cout << "Root DOES NOT exist. Return empty vector.\n";
						return (string)tmpI;
					}


					//������� � ����, ���� ������� �������� �� ���������
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


						//����� ��� ������ �����������,
						//������ ��� � ��������������� ���� ��� ����� ��
						newNode->dnf = currentNode->dnf;
						newNode->count = currentNode->count;
						newNode->varsToBeConsidered = currentNode->varsToBeConsidered;
						newNode->rowsToBeConsidered = currentNode->rowsToBeConsidered;
						newNode->Solution = currentNode->Solution;


						//������� ���� ���� � ����
						mystack.push(newNode);

					}


					posA = component.n_comp;
					currentNode->var = posA;



					//��������� �������� � ����
					//
					//�������, ��� ���� �������� � ������ ����� ��� ���� �� ��������� ������� 

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

			//���� �� ��������, � ������, ����� ���� �� ��������, � ���������������� ����, �������������, � �������������, 
			//����� ��������� ���������� ��������� ���������� ������������ ��������� ����������.
			//�����, �����, ����� ��������� ������ ������������� ���������. 
			//���� ������� �������, �� ����� ������ ������������.
			//� ����� ������� �� �� �����������, � ������ ��������� ����������, 
			//�� ������, ��� ����� ���������� ���� ���������, � �������������,
			//���������� ��� ����� ��������������� ��������. ������ ���� ����� ��� ����������� �������.
			else
			{
				

				

				// ������������ ������� �������� ��������

				posA = currentNode->var;
				

				//�������� ����� ���, ����� � ���������� ����


				currentNode->varsToBeConsidered[posA] = 0;


				//��������� ������!
				
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


				//���� ��� ��������� �����, �� ������ ������
				//������� "����� ������", ��� ������ � ���������� �����
				//cout << "Solution:" << endl;

				

				if (currentNode->count == 0)
				{
					return (string)currentNode->Solution;
				}



				//���� �����, �� ���������� ��������������� ��������, � ��������� ������ ������������� �����.
				currentNode->fixVal_0 = false;
				currentNode->fixVal_1 = false;


			}
		}
		
	}

};


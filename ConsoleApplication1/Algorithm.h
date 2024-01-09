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




struct Solution
{
	//root
	//��� ���-������, ���������� ��
};


class Algorithm
{
	Heuristics* heuristics;
	//��������� �� ������� � � ������, ����� ����� ���� "�����������"

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

	//Interval** newDnf = new Interval * [numrows];
	
	int row_value;

	for(int i = 0; i < sz; i++)
	{
		
		
		if (!node->rowsToBeConsidered[i])
		{
			//break;
			continue;
		}
		
		
		row_value = node->dnf[i]->getValue(component.n_comp) - '0';
		//cout << row_value << endl;

		//������, ������� �� "���������" �� �������
		if (row_value == !component.val_comp)
		{
			//cout << i << endl;
			//newDnf[i] = setIntevals[i];
			//node->dnf[i] = nullptr;
			node->rowsToBeConsidered[i] = 0;
			node->count--;
		}

	
	}



	//cout << "count = " << node->count << endl;
	
}












	Component checkHeuristicRules(NodeBoolTree*& node, int sz, int countVar)
	{	
		Component component;
		
		component = heuristics->checkRules(node, sz, countVar);

		return component;

		//������ ���� ������ return heuristics->checkRules()
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

		//bool isEmptyRow = false;

		int i, j;

		//��� �� �������
		for (i = 0; i < sz; i++)
		{
			
			if (!node->rowsToBeConsidered[i])
			{
				//break;
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
						//isEmptyRow = false;
						break;
					}
				}		
			}

			//���� ��� ������ ������� ������� �� ��������
			if (j == countVar)
			{
				
				//cout << "rule 2!" << endl;
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
				//break;
				continue;
			}
			
			Interval tmpI;

			BBV tmpVec = node->dnf[i]->vec; //getVec
			BBV tmpDnc = node->varsToBeConsidered;

			tmpDnc = ~tmpDnc;
			tmpDnc = tmpDnc | node->dnf[i]->dnc; //getDnc

			tmpI.setVec(tmpVec);
			//tmpI.vec = tmpVec;
			tmpI.setDnc(tmpDnc);

			//cout << "tmpI" << endl;
			//cout << tmpI.getVec() << endl;
			//cout << tmpI.getDnc() << endl;
			//cout << (string)tmpI << endl;
			
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
							//cout << "rule 1: " << i << endl;

							component.n_comp = j;
							component.val_comp = !(node->dnf[i]->getVec()[j]);

							return component;

						}
					}
					
				}

			}

		}


		//cout << "rule_1" << endl;
		//component.Print();

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

		//BBV Zero(countVar), notZero;
		//notZero = ~Zero;

		//����� �������� �� ������ ����� ������� ����

		bool isNotDefined; //rule 3

		bool isOrth0, isOrth1; //rule 4
		//cout << isNotDefined << endl;
		//cout << isOrth0 << endl;
		//cout << isOrth1 << endl;

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
						//break;
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

				//cout << "isNotDefined: " << isNotDefined << endl;
				//cout << "isOrth0:      " << isOrth0 << endl;
				//cout << "isOrth1:      " << isOrth1 << endl; //�� ���������


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
					//component.n_comp = j;
					//component.val_comp = 0; //����� ���� �����, ��������� 0 //�� ���������
					//component.val_comp = 0; //����� ���� �����, ��������� 0

					//return component;

					notDefinedIdx = j;
				}
			}

		}

		/*
		cout << "isNotDefined: "<< isNotDefined << endl;
		cout << "isOrth0:      " << isOrth0 << endl;
		cout << "isOrth1:      " << isOrth1 << endl; //�� ���������

		if (isOrth1 == Zero)
		{
			cout << "True" << endl;
		}

		*/

		/*
		cout << "rule_3-4" << endl;

		cout << "isNotDefined: " << isNotDefined << endl;
		cout << "isOrth0:      " << isOrth0 << endl;
		cout << "isOrth1:      " << isOrth1 << endl;
		*/


		//�����, ����������, ����� ��������� �������. �������
		if (notDefinedIdx != -1)
		{
			component.n_comp = notDefinedIdx;
			//component.val_comp = 0; //����� ���� �����, ��������� 0 //�� ���������
			component.val_comp = 0; //����� ���� �����, ��������� 0
		}


		//component.Print();

		return component;

	}





	string findRoot(BoolEquation& equation)
	{
		//����� ������

		int sz = equation.getNumRows();
		int countVar = equation.getCountVar();

		
		//
		Interval** setIntevals = equation.getSetIntevals();

		Interval** setTest = new Interval * [sz];

		for (int ix = 0; ix < sz; ix++)
		{
			//setTest[ix] = nullptr;
			setTest[ix] = setIntevals[ix];
		}



		//����� ������. ���������� ����� ��� ���� ����������
		cout << "BEGIN TREETRAVERSAL" << endl;

		string root;


		root = TreeTraversal(setTest, sz, countVar);


		cout << "END TREETRAVERSAL" << endl;

		/*
		//������� setIntevals ���������
		for (int ix = 0; ix < sz; ix++)
		{
			delete setIntevals[ix];
			setIntevals[ix] = nullptr;
		}
		*/
		//����� �������� setIntevals, � ��������� setTest ��� ������ �� ��������������, ��� �� �����������.
		//����� �������� ��� ������
		for (int ix = 0; ix < sz; ix++)
		{
			setTest[ix] = nullptr;
		}

		return root;
	}



	//��������� ����������, ����� �������, ������ ������� (���-�� ����������)
	string TreeTraversal(Interval** setIntevals, size_t size, unsigned countVar)
	{

		//������ �������� �������� ������, if (setIntervals), � ���������, � ��������� �������

		std::stack<NodeBoolTree*> mystack;

		bool continueTravesal = true;

		//bool isNotExist = false;

		NodeBoolTree* currentNode = nullptr;

		//����� ������� ����������
		int posA = -1; //int posA = 0;


		//int varlen = setIntevals[0]->length();
		//BBV varsToBeConsidered(varlen);




		BBV varsToBeConsidered(countVar);

		varsToBeConsidered = ~varsToBeConsidered;
		//cout << "varsToBeConsidered: " << varsToBeConsidered << endl;


		BBV rowsToBeConsidered(size);
		rowsToBeConsidered = ~rowsToBeConsidered;
		//cout << "rowsToBeConsidered: " << rowsToBeConsidered << endl;

		/*
		//varsToBeConsidered = ~varsToBeConsidered;
		for (int i = 0; i < countVar; i++)
		{
			varsToBeConsidered[i] = 1;
		}
		*/
		


		//������� ����� ������
		BBV Nol(countVar);

		//BBV Nol(varlen);


		//�� ������� ������� ������. ������ � ������ ��� ���������� ��������� �������� 1.
		BBV NotNol(countVar);
		NotNol = ~Nol;

		//cout << NotNol;

		//������� ��������
		Interval tmpI(countVar);
		tmpI.setDnc(NotNol);

		//cout << (string)tmpI;

		
		//������� ��������� ���� ������ ������
		//�������������� ���� dnf(������� ��������� ���������� ��� ������� ����). 
		//� ��������� ������ ������� ��� �������� ���������.


		currentNode = new NodeBoolTree(countVar, size); // currentNode = new NodeBoolTree();

		//currentNode = new NodeBoolTree(countVar);
		//currentNode->dnf = new Interval * [size]; //������ ����������

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

			//cout << setIntevals[ix] << endl;
			//cout<< (string)(*currentNode->dnf[ix]) << endl;
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
		
		//cout << (string)currentNode->Solution << endl;

		//cout << currentNode->dnf[0]->length() << endl;

		currentNode->varsToBeConsidered = varsToBeConsidered;

		//cout << currentNode->varsToBeConsidered << endl;

		currentNode->rowsToBeConsidered = rowsToBeConsidered;

		//cout << currentNode->rowsToBeConsidered << endl;

		Component component;

		while (continueTravesal)
		{
			//cout << "continueTravesal: " << continueTravesal << endl;



			/**/

			//��� �������, ������� ��������, �� ��, ��� ��� ������� ���� �� ���������� ������ 
			if (currentNode && (!currentNode->fixVal_0 && !currentNode->fixVal_1)) 
			{
				//�����, ���������� ������������� ������� ��������� ���������� �� ������� ������ 
				// ���������� ��������� � �������� �������� ��� ��������� ���������� ���������.

				/*
				� ������������ ����������, ��� ��� ��� ������ ������, �� ���������� ����� ���������, 
				�.�. ��������������� ��� ������ ���������� ������������ ���������.
				� ��������, ������, ��� ��������� ���������� ��������� �������� 0, 
				� ���� �� ����, �� � ������ ��������� �������� 1 ������� � ����.
				*/

				//�����, �������  - ���� �������������� ����, � ������� ������������� ��������� ����������
				//(��� ��� ��  ��������������� ���������� ���������� ���������) 
				//���� ��� ������� ����������� ��, ������� ���� ��� �� �����, ��� ��� �� ��� ��� ������� �����������. 
				//��� �������, � �� ����� ������� �����.   



				//currentNode->ToBeConsidered == 000000000000....
				//if (currentNode->var == countVar - 1)	//�������� �� "���� �� ��� �����������", � ����� ���
				if (currentNode->varsToBeConsidered == Nol)
				{
					

					delete currentNode;

					currentNode = nullptr;

					//���� ���� ����, �� ���������� �����
					if (mystack.empty())
					{
						continueTravesal = false;
						return (string)tmpI;
						//��� ����� ����, ��� �� ����� ������?
					}

					//����� ������� ������� �� �����
					else
					{
						
						//�������� ����� ���������� ��������
						currentNode = mystack.top();

						//cout << "cN fr stack  " << (string)(*currentNode->dnf[0]) << endl;

						//�������� �� �����.
						mystack.pop();

						//����� ���������� � ����, �.�. ����� ����������, � ����� �������� ���� �������������  
						//cout << "---->FromSteck" << '(' << currentNode->var << ',' << "L(" << currentNode->fixVal_0 << ")," << "R(" << currentNode->fixVal_1 << ")" << endl;
						
						/*
						cout << "Node->dnf:" << endl;
						for (int i = 0; i < size; i++)
						{

							if (!currentNode->dnf[i])
							{
								cout << "+" << endl;
							}

							else
							{
								cout << (string)*currentNode->dnf[i] << endl;
							}
						}
						cout << "end dnf" << endl;

						*/

					}
						
				}

				// ����� ��������� ���������� ������, ��� ��� ������� ��������������� ���, �� �������� ��������� 
				else
				{
					
					//����� ���������� � Ũ �������� �� ���������

					component = checkAlgorithmRules(currentNode, size, countVar);

					//���� ��������� ������ �������, ��
					//������� "����� ���" ���������� �����

					if (component.n_comp == -2)
					{
						//cout << "Rule 2" << endl;

						//continueTravesal = false;
						//break;

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
							//currentNode->fixVal_0 = true;
							//currentNode->Solution.setValue('0', posA);

							//��������� ��������������� �������� 
							newNode->fixVal_1 = true;
						}

						if (component.val_comp == 1)
						{
							//currentNode->fixVal_1 = true;
							//currentNode->Solution.setValue('1', posA);

							//��������� ��������������� �������� 
							newNode->fixVal_0 = true;
						}


						//����� ��� ������ �����������,
						//������ ��� � ��������������� ���� ��� ����� ��
						newNode->dnf = currentNode->dnf;
						newNode->count = currentNode->count;
						newNode->varsToBeConsidered = currentNode->varsToBeConsidered;
						newNode->rowsToBeConsidered = currentNode->rowsToBeConsidered;
						newNode->Solution = currentNode->Solution;


						//������ ���� ���� � ����
						mystack.push(newNode);
						//cout << "New Node with Var" << '(' << posA << ',' << "L(" << currentNode->fixVal_0 << ")," << "R(" << currentNode->fixVal_1 << ")" << endl;

						/*
						cout << "Node->dnf:" << endl;
						for (int i = 0; i < size; i++)
						{

							if (!currentNode->dnf[i])
							{
								cout << "+" << endl;
							}

							else
							{
								cout << (string)*currentNode->dnf[i] << endl;
							}
						}
						cout << "end dnf" << endl;

						*/

					}


					//component.Print();

					//posA++;
					posA = component.n_comp;
					currentNode->var = posA;



					//��������� �������� � ����
					//
					//�������, ��� ���� �������� � ������ ����� ��� ���� �� ��������� ������� 

					if (component.val_comp == 0)
					{
						currentNode->fixVal_0 = true;
						//currentNode->Solution.setValue('0', posA);
					}

					if (component.val_comp == 1)
					{
						currentNode->fixVal_1 = true;
						//currentNode->Solution.setValue('1', posA);
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
				//cout << "Simplification " << endl;
				posA = currentNode->var;
				//cout << "Node" << '(' << posA << ',' << "L(" << currentNode->fixVal_0 << ")," << "R(" << currentNode->fixVal_1 << ")" << endl;

				//� �����, ����� ����� ������� ������� � ��� , ����� �� ��� �������� ���������� ������.
				

				//�������� ����� ���, ����� � ���������� ����


				currentNode->varsToBeConsidered[posA] = 0;
				//cout << "varsToBeConsidered: " << endl;
				//cout << currentNode->varsToBeConsidered << endl;





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
				*/


				//���� ��� ��������� �����, �� ������ ������
				//������� "����� ������", ��� ������ � ���������� �����
				//cout << "Solution:" << endl;

				//cout << (string)currentNode->Solution << endl;

				if (currentNode->count == 0)
				{
					//cout << "ROOT:    "<< (string)currentNode->Solution << endl;
					//continueTravesal = false;

					return (string)currentNode->Solution;
				}



				//���� �����, �� ���������� ��������������� ��������, � ��������� ������ ������������� �����.
				currentNode->fixVal_0 = false;
				currentNode->fixVal_1 = false;


			}
		}
		
	}

};


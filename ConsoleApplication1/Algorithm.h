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




//�����, ����������� �������� ������ ����� ��������� D=0

class Algorithm
{
	//��������� ���������
	Heuristics* heuristics; 

	//���������� ��������, ���������� ��� ��������� ������ findRoot
	int lastCountOfIterations; 
	
	
public:

	Algorithm()
		: lastCountOfIterations(0)
	{
		heuristics = nullptr;
	}

	//���������� ��������� ���������� ��������
	int getLastCountOfIterations()
	{
		return 
			lastCountOfIterations;
	}

	//��������� ���������
	void setHeuristics(Heuristics* h)
	{
		heuristics = h;	
	}

	//���������� �������� ���������
	string getHeuristicsName()
	{
		return heuristics->getName();
	}

	//����� ���������� � ������� ������������� ������
	Component checkHeuristicRules(NodeBoolTree*& node, int sz, int countVar)
	{	

		return
			heuristics->checkRules(node, sz, countVar);

	}

	//�������� ������ ��������� � ����� ����������
	//���������� ������ ������ Component (����� ����������, �������� ����������)
	//���� ����� ���������� (��������� ���� � ����) ����� -1, ������, �� ���� �� ������ �� ���������
	//���� ����� ���������� ����� -2, �� ����������� ������� ������� ��������� (������ �� �������������� ���������)
	Component checkAlgorithmRules(NodeBoolTree*& node, int sz, int countVar)
	{
		Component component;

		// ������� 1 � 2
		component = scanMatrixHorizontal(node, sz, countVar);

		if (component.n_comp == -1)
		{
			// ������� 3 � 4
			component = scanMatrixVertical(node, sz, countVar);

		}


		return component;
		
	}

	//������ ������� ���������
	Component rule_2(NodeBoolTree*& node, int sz, int countVar);

	//������ ������� ���������
	Component rule_1(NodeBoolTree*& node, int sz, int countVar);

	//������ ����� ������� (������� 1-2)
	Component scanMatrixHorizontal(NodeBoolTree*& node, int sz, int countVar);

	//������ �������� ������� (������� 3-4)
	Component scanMatrixVertical(NodeBoolTree*& node, int sz, int countVar);

	//�������� ����� (������� 5)
	void Simplification(Component component, NodeBoolTree*& node, int sz);

	//����� �����, ���������� ������� TreeTraversal
	string findRoot(BoolEquation& equation);

	//����� �������; ���� ����� �� ����������, ���������� ������, ��� ���������� �������� �� ����������
	string TreeTraversal(Interval** setIntevals, size_t size, unsigned countVar);

};


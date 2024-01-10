#pragma once

#include "BBV.h"
#include "Interval.h"
#include "NodeBoolTree.h"
#include "BoolEquation.h"
#include "Component.h"


#include <iostream>

using namespace std;



//�����-��������� ���������
class Heuristics
{
public:
	virtual Component checkRules(NodeBoolTree*& node, int sz, int countVar) = 0; //����� ����������
    virtual string getName() = 0; //��������� �������� ���������
};

//greedHeuristics
//� ��������, ����� �� ���� �� ������ ��������� �� �����������, ��������������� ���������� ���������:
//1. ����� ������.
//�������� ������ � ����������� ������ .
//2. ����� ����������.
//������������� ������������ ���������� ��������� ������, ������������, 
//������� ����� � ������ � ������ ������� � �������� �� ����������, 
//� ������� ������� ���������� ���������� ����� � ����������� ����������.

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
//� ��������, ����� �� ���� �� ������ ��������� �� �����������, 
//�������� �������� ������������ ������� � ����������� ���������� ��������,
//��������������� �������� ����������� ��������� � ������ �������.

class simpleHeuristics : public Heuristics 
{     
public:

	string getName()
	{
		return "simpleHeuristics";
	}
	
	Component checkRules(NodeBoolTree*& node, int sz, int countVar);
};
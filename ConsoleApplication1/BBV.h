#pragma once


#include <iostream>
using namespace std;

typedef unsigned char byte;

class X
{
	byte* ptr;//��������� �� ������, ��� �������� ������ ���
	int index;//����� ���� � ������
public:
	X();//����������� �� ��������
	X(byte* vec, int k);//����������� � �����������
	X operator=(int k);//���������� ��������� ����������
	operator int();//���������� ����
	X operator=(X& v);
};


class BBV
{
	friend X;
	byte* vec;//��������� �� ������ �����
	int size;//���������� �����
	int len;//���������� ��������
public:
	~BBV();//����������
	BBV();//����������� �� ��������
	explicit BBV(int size);//����������� � ����������. � ������� ��������� ������ ����� �������
	explicit BBV(const char* str);//����������� � ����������. � �������� ��������� ������, ������� ����������� � ������
	BBV(BBV& V);//����������� �����������
	void Init(const char* str);//����� ������������� ������� �� ������
	void Set0(int k);//����� ��������� k-��� ������� � 0
	void Set1(int k);//����� ��������� k-��� ������� � 1
	BBV operator = (BBV& V);//���������� ��������� ����������
	BBV operator = (const char* str);//���������� ��������� ���������� ��� ������ �� ��������
	bool operator ==(BBV& V);//���������� ��������� ���������
	bool operator !=(BBV& V);//���������� �����������
	BBV operator | (BBV& V);//���������� �������� ��������
	BBV operator & (BBV& V);//���������� ��������� ��������
	BBV operator ^ (BBV& V);//�������� ����� �� ������ ���
	BBV operator ~ ();//�������� �������
	BBV operator >> (int k);//����� ������
	BBV operator << (int k);//����� �����
	X operator [] (int k);//���������� �������� []
	operator char* ();//���������� �������� ���
	int getWeight();//����� ��� ��������� ���� �������
	int getSize();//����� ��� ��������� ������� �������
	friend ostream& operator << (ostream& r, BBV& V);
	friend istream& operator >> (istream& r, BBV& V);
};

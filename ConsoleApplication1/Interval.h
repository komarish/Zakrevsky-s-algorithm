#pragma once

#include "BBV.h"


class Interval
{
	BBV vec;
	BBV dnc;
public:
	explicit Interval(size_t len = 8); //!
	Interval(const char* vec_in, const char* dnc_in);
	explicit Interval(const char* vector);
	Interval(BBV& vec_in, BBV& dnc_in);
	Interval& operator=(Interval& ibv);
	bool operator==(Interval& ibv);
	bool operator!=(Interval& ibv);
	operator string();
	int length(); // ����� ��� ���������� ���������� 
	int rang(); // ���� ���������
	bool isOrthogonal(Interval& ibv);
	char getValue(int ix); // �������� i �� �������� ���������� ���������
	BBV getVec();
	BBV getDnc();
	void setVec(BBV& v);
	void setDnc(BBV& v);
	void setValue(char value, int ix); // ���������� i�� �������� ���������
};



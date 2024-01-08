#pragma once

#include "BBV.h"


class Interval
{
	
public:
	BBV vec;
	BBV dnc;
public:
	explicit Interval(size_t len = 8); //!
	Interval(const char* vec_in, const char* dnc_in);
	explicit Interval(const char* vector);
	Interval(BBV& vec_in, BBV& dnc_in);
	void setInterval(BBV& vec, BBV& dnc);
	Interval& operator=(Interval& ibv);
	bool operator==(Interval& ibv);
	bool operator!=(Interval& ibv);

	operator string();

	int length(); // Длина или количество переменных 
	int rang();// Ранг интервала
	bool isOrthogonal(Interval& ibv);
	Interval& mergeInterval(Interval& ibv); // Построение пересечения интервалов
	bool isIntersection(Interval& ibv);
	bool isAbsorb(Interval& ibv);
	char getValue(int ix);//Получить i ое значение компоненты интервала
	BBV getVec();
	BBV getDnc();
	void setVec(BBV& v);
	void setDnc(BBV& v);

	void setValue(char value, int ix); //Установить iое значение интервала

};



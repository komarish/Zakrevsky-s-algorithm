#include "Interval.h"



Interval::Interval(size_t len)
{
	
	//cout << "Interval(size_t len), len =" << len << endl;
	vec = BBV(len);
	dnc = BBV(len);
}

Interval::Interval(const char* vec_in, const char* dnc_in)
{
	//Проверяем , если строки не пустые и их длины совпадают, то инициализируем вектора, иначе ошибка и в этом случае создаем нулевые веткора мин длины(1 байт).
	if (vec_in && dnc_in && strlen(vec_in) == strlen(dnc_in))
	{
		vec = BBV(vec_in);
		dnc = BBV(dnc_in);
	}
	else
	{
		vec = BBV(8);
		dnc = BBV(8);
	}

}

Interval::Interval(const char* vector)
{
	if (vector)
	{
		size_t sz = strlen(vector);
		vec = BBV(sz);
		dnc = BBV(sz);
		int ix = 0;
		while (ix < sz)
		{
			if (vector[ix] == '-')
				dnc[ix] = 1;
			else
				if (vector[ix] == '1')
					vec[ix] = 1;
			ix++;
		}
	}
}




Interval::Interval(string& s)
{
	size_t sz = s.length();
	
	if (sz)
	{
		vec = BBV(sz);
		dnc = BBV(sz);
		int ix = 0;
		while (ix < sz)
		{
			if (s[ix] == '-')
				dnc[ix] = 1;
			else
				if (s[ix] == '1')
					vec[ix] = 1;
			ix++;
		}
	}
}





Interval::Interval(BBV& vec_in, BBV& dnc_in)
{
	vec = vec_in;
	dnc = dnc_in;

}

void Interval::setInterval(BBV& vec, BBV& dnc)
{
}

Interval& Interval::operator=(Interval& ibv)
{
	// TODO: вставьте здесь оператор return

	vec = ibv.vec;
	dnc = ibv.dnc;

	return *this;
}

bool Interval::operator==(Interval& ibv)
{
	if (vec == ibv.vec && dnc == ibv.dnc)
		return true;
	return false;
}

bool Interval::operator!=(Interval& ibv)
{
	if (vec != ibv.vec || dnc != ibv.dnc)
		return true;
	return false;
}

Interval::operator string()
{
	size_t sz = vec.getSize();
	string str(vec.getSize(), '0');

	for (int ix = 0; ix < sz; ix++)
	{
		str[ix] = getValue(ix);
	}

	return str;
}

int Interval::length()
{
	return vec.getSize();
}

int Interval::rang()
{
	//Определяем ранг интервала
	return (vec.getSize() - dnc.getWeight());
}

//Реализуем в соответсвии с примером из лекций
/*

bool IsOrthogonal(struct Interval u, struct Interval v )
{
unsigned long tmpUV, tmpU, tmpV;
tmpUV = v.dnc | u.dnc;
tmpU = u.vec | tmpUV;
tmpV = v.vec | tmpUV;
if ((tmpU ^ tmpV) != 0)
return true;
return false;
}

*/

bool Interval::isOrthogonal(Interval& ibv)
{
	//cout << (string)(*this) << endl;
	//cout << (string)ibv << endl;

	BBV zero(vec.getSize());

	BBV tmpUV(zero);
	BBV tmpU(zero);
	BBV tmpV(zero);

	BBV answer(zero);

	tmpUV = dnc | ibv.dnc;
	//Output vector for debug

	//cout << (string)tmpUV << endl;

	tmpU = vec | tmpUV;
	//Output vector for debug
	//cout << (string)tmpU << endl;

	tmpV = ibv.vec | tmpUV;
	//Output vector for debug

	//cout << (string)tmpV << endl;

	answer = (tmpU ^ tmpV);
	//Output vector for debug

	//cout << "answer"<< (string)answer << endl;

	if (answer != zero)
		return true;

	return false;

}

Interval& Interval::mergeInterval(Interval& ibv)
{

	//Если требуется, реализовать самостоятельно
	// TODO: вставьте здесь оператор return

	return *this;
}



bool Interval::isIntersection(Interval& ibv)
{
	//Если требуется, реализовать самостоятельно

	return false;
}

bool Interval::isAbsorb(Interval& ibv)
{
	//Если требуется, реализовать самостоятельно

	return false;
}

char Interval::getValue(int ix)
{
	if (ix < 0 || ix > vec.getSize())
		throw "Out of range";

	if (dnc[ix] == 1)
		return '-';

	if (vec[ix] == 1)
		return '1';
	return '0';
}

BBV Interval::getVec()
{
	return this->vec;
}

BBV Interval::getDnc()
{
	return this->dnc;
}

void Interval::setVec(BBV& v)
{
	vec = v;
}

void Interval::setDnc(BBV& v)
{
	dnc = v;
}

void Interval::setValue(char value, int ix)
{
	//cout << "vec.size = " << vec.getSize() << " ix = " << ix << endl;

	
	if (ix < 0 || ix > vec.getSize())
		throw "Out of range";
	if (value == '-')
	{
		dnc[ix] = 1;
		vec[ix] = 0;
	}
	else
		if (value == '0') {
			vec[ix] = 0; //
			dnc[ix] = 0;
		}
		else
		{
			vec[ix] = 1;
			dnc[ix] = 0;
		}
}

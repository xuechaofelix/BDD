#ifndef SATISFY_RESULT_H_
#define SATISFY_RESULT_H_

#include <iostream>

using namespace std;

class Satisfy_Result
{
private:
	int order;
	bool value;

public:
	Satisfy_Result(){}
	Satisfy_Result(int o,int v)
	{
		this->order = o;
		this->value = v;
	}

	int Get_Order() const {return order;}
	bool Get_Value() const {return value;}

	void Set_Order(int o){this->order = o;}
	void Set_Value(bool v){this->value = v;}

	friend ostream & operator<<(ostream &out, const Satisfy_Result &obj);

};
ostream & operator<<(ostream &out, const Satisfy_Result &obj)
{
	out <<"<x_" <<obj.Get_Order() << ":" << obj.Get_Value()<<">";
	return out;
}
#endif
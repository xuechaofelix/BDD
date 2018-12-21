#ifndef KEY_H_
#define KEY_H_

#include <iostream>
#include "node.h"

using namespace std;

template<class T>
class Key 
{
private:
	T value;
	Node * first;
	Node * second;
	size_t hash_value;

	size_t Hash_code();
public:
	Key(T val, Node * fir, Node * sec);
	Key(){this->hash_value = 0;}
	bool operator ==(const Key<T> & k) const;
	Key<T> & operator =(const Key<T> & k);

	T Get_Value() const;
	Node * Get_First_Node() const;
	Node * Get_Second_Node() const;
	size_t Get_Hash_Value() const;

	void Show();

};
/*********************************************************************/
template<class T>
Key<T>::Key(T val, Node * fir, Node * sec)
{{
	this -> value = val;
	this -> first = fir;
	this -> second = sec;
	this -> hash_value = Hash_code();
				// val +
	  	// 	   reinterpret_cast<long>(fir)-
	  	// 	   reinterpret_cast<long>(sec);

}}
template<class T>
bool Key<T>::operator ==(const Key<T> & k) const
{{
	return  (this-> value == k.Get_Value())&&
			(this-> first == k.Get_First_Node()) &&
			(this-> second == k.Get_Second_Node());
}}
template<class T>
Key<T> & Key<T>::operator =(const Key<T> & k)
{{
	this -> value = k.Get_Value();
	this -> first = k.Get_First_Node();
	this -> second = k.Get_Second_Node();
	return *this;
}}

template<class T>
size_t Key<T>::Hash_code()
{{
	size_t result=reinterpret_cast<long>(first);
	size_t tmp = reinterpret_cast<long>(second);
	while(tmp !=0)
	{
		int num = tmp%10;
		result = result*10+num;
		tmp = tmp/10;
	}
	tmp = value;
	while(tmp !=0)
	{
		int num = tmp%10;
		result = result*10+num;
		tmp = tmp/10;
	}

	return result;
}}
template<class T>
T Key<T>::Get_Value() const
{{
	return this->value;
}}

template<class T>
Node * Key<T>::Get_First_Node() const
{{
	return this->first;
}}

template<class T>
Node * Key<T>::Get_Second_Node() const
{{
	return this->second;
}}

template<class T>
size_t Key<T>::Get_Hash_Value() const
{{
	return this->hash_value;
}}

template<class T>
void Key<T>::Show()
{{
	cout<<this->value<<"---"<<this->first->Get_Name()<<"---"<<this->second->Get_Name()<<endl;
}}



#endif
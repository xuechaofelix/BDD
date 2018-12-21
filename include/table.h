#ifndef TABLE_H_
#define TABLE_H_
#include <iostream>
#include "util.h"

using namespace std;
using namespace __gnu_cxx;

template<class T>  
size_t Time33Hash(const T *str)  
{  
    register size_t hash = 0;  
    while (size_t ch = (size_t)*str++)  
    {         
        hash = hash * 33 + ch;            
    }  
    return hash;  
}  

template<class T> 
struct Keyhash
{ 
	size_t operator()( const T & x ) const 
	{ 
		return x.Get_Hash_Value();
	  	// return x.Get_Value() +
	  	// 	   reinterpret_cast<long>(x.Get_First_Node())+
	  	// 	   reinterpret_cast<long>(x.Get_Second_Node());
	  		   //Time33Hash(toString<void*>(x.Get_First_Node()).c_str())+
	  		   //Time33Hash(toString<void*>(x.Get_Second_Node()).c_str()); 
	} 
};

template<> 
struct Keyhash< std::string > 
  { 
    size_t operator()( const std::string& x ) const 
    { 
    	return Time33Hash(x.c_str());
      //return hash< const char* >()( x.c_str() ); 
    } 
  };

template<class T> 
struct Keyequal 
{ 
	bool operator()( const T& x1 , const T& x2) const 
	{ 
		return x1==x2; 
	} 
};

template<
	template<	class _Key, 
				class _Tp,
          		class _HashFcn = Keyhash<_Key>,   // defined in sparseconfig.h
          		class _EqualKey = Keyequal<_Key>,
          		class _Alloc = allocator<std::pair<const _Key, _Tp> > 
			> 
	class T1 ,
	class T2 ,
	class T3 >
class Table
{
protected:
	T1<T2,T3 *> table;
public:
	Table(){}
	Table(T2 empty_key,T2 deleted_key)
	{
		this->table.set_empty_key(empty_key);
		this->table.set_deleted_key(deleted_key);
	}
	void add(T2 key, T3* element);
	bool remove(T2 key);
	bool contains(T2 key);
	T3* get(T2 key);

	int size();
};

/************************implement Table**************************************/
template<
	template<	class _Key, 
				class _Tp,
          		class _HashFcn = Keyhash<_Key>,   // defined in sparseconfig.h
          		class _EqualKey = Keyequal<_Key>,
          		class _Alloc = allocator<std::pair<const _Key, _Tp> > 
			> 
	class T1 ,
	class T2 ,
	class T3 >
void Table<T1,T2,T3>::add(T2 key, T3* element)
{{
	//key.Show();
	//element->Show();
	this->table.insert(make_pair(key,element));
	//this->table.insert(key,element);
	//this->table[key] = element;
}}
template<
	template<	class _Key, 
				class _Tp,
          		class _HashFcn = Keyhash<_Key>,   // defined in sparseconfig.h
          		class _EqualKey = Keyequal<_Key>,
          		class _Alloc = allocator<std::pair<const _Key, _Tp> > 
			> 
	class T1 ,
	class T2 ,
	class T3 >
bool Table<T1,T2,T3>::remove(T2 key)
{{
	return this->table.erase(key);
}}
template<
	template<	class _Key, 
				class _Tp,
          		class _HashFcn = Keyhash<_Key>,   // defined in sparseconfig.h
          		class _EqualKey = Keyequal<_Key>,
          		class _Alloc = allocator<std::pair<const _Key, _Tp> > 
			> 
	class T1 ,
	class T2 ,
	class T3 >
bool Table<T1,T2,T3>::contains(T2 key)
{{
	typename T1< T2 ,T3*>::iterator iter = this->table.find(key);
	if(iter == this->table.end())
	{
		return false;
	}
	else
	{
		return true;
	}
	// if (this->table.find(key) == NULL)
	// 	return false;
	// else
	// 	return true;
}}
template<
	template<	class _Key, 
				class _Tp,
          		class _HashFcn = Keyhash<_Key>,   // defined in sparseconfig.h
          		class _EqualKey = Keyequal<_Key>,
          		class _Alloc = allocator<std::pair<const _Key, _Tp> > 
			> 
	class T1 ,
	class T2 ,
	class T3 >
T3* Table<T1,T2,T3>::get(T2 key)
{{
	typename T1<T2,T3*>::iterator iter = this->table.find(key);
	if(iter == this->table.end())
	{
		return NULL;
	}
	else
	{
		return iter->second;
	}
	//return this->table.find(key);
}}
template<
	template<	class _Key, 
				class _Tp,
          		class _HashFcn = Keyhash<_Key>,   // defined in sparseconfig.h
          		class _EqualKey = Keyequal<_Key>,
          		class _Alloc = allocator<std::pair<const _Key, _Tp> > 
			> 
	class T1 ,
	class T2 ,
	class T3 >
int Table<T1,T2,T3>::size()
{{
	return this->table.size();
}}

#endif
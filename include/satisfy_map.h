#ifndef SATISFY_MAP_H_
#define SATISFY_MAP_H_
#include <google/dense_hash_map>
#include <ext/hash_map>
#include "table.h"
#include "key.h"

using namespace std;
using namespace __gnu_cxx;


template<
	template<	typename T_Key,
				typename T_Type,
				typename HashFcn = Keyhash<T_Key>,
				typename EqualKey = Keyequal<T_Key>,
				typename Alloc = allocator<pair <const T_Key, T_Type> > 
			> 
	class T,
	class Value
		>
class Satisfy_Map:
	public Table<T,Key<int>, Value>
{
public:
	Satisfy_Map();
	
};
/**********************************************************/
template<class Value>
class Satisfy_Map<google::dense_hash_map,Value>:
	public Table<google::dense_hash_map,Key<int>, Value>
{
public:
	Satisfy_Map();
	
};
/********************************************************/
template<
	template<	typename T_Key,
				typename T_Type,
				typename HashFcn = Keyhash<T_Key>,
				typename EqualKey = Keyequal<T_Key>,
				typename Alloc = allocator<pair <const T_Key, T_Type> > 
			> 
	class T,
	class Value
		>
Satisfy_Map<T,Value>::Satisfy_Map()
{{
	//this->unique_table.set_empty_key(string());
}}

template<class Value>
Satisfy_Map<google::dense_hash_map,Value>::Satisfy_Map()
	:Table<google::dense_hash_map,Key<int>, Value >(Key<int>(-1,NULL,NULL),Key<int>(-2,NULL,NULL))
{{
	//this->unique_table.set_empty_key(string());
}}




#endif
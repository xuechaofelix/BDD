#ifndef UNIQUE_TABLE_H_
#define UNIQUE_TABLE_H_
#include <google/dense_hash_map>
#include <ext/hash_map>
#include "node.h"
#include "table.h"

using namespace std;
using namespace __gnu_cxx;


template<
	template<	typename T_Key,
				typename T_Type,
				typename HashFcn = Keyhash<T_Key>,
				typename EqualKey = Keyequal<T_Key>,
				typename Alloc = allocator<pair <const T_Key, T_Type> > 
			> 
	class T
		>
class Unique_Table:
	public Table<T,Key<int>, Node >
{
public:
	Unique_Table();
	
};

template<>
Unique_Table<google::dense_hash_map>::Unique_Table()
	:Table<google::dense_hash_map,Key<int>, Node>(Key<int>(-3,NULL,NULL),Key<int>(-4,NULL,NULL))
{{
	//this->unique_table.set_empty_key(string());
}}

template<
	template<	typename T_Key,
				typename T_Type,
				typename HashFcn = Keyhash<T_Key>,
				typename EqualKey = Keyequal<T_Key>,
				typename Alloc = allocator<pair <const T_Key, T_Type> > 
			> 
	class T
		>
Unique_Table<T>::Unique_Table()
{{
	//this->unique_table.set_empty_key(string());
}}


#endif
#ifndef CACHE_TABLE_H_
#define CACHE_TABLE_H_

#include <ext/hash_map>
#include <google/dense_hash_map>

#include "key.h"
#include "node.h"
#include "table.h"

using namespace std;

enum Operation {
     EMPTY_,DELETE_,
    AND_,OR_,XOR_,NOT_
};

template<
	template<	typename T_Key,
				typename T_Type,
				typename HashFcn = Keyhash<T_Key>,
				typename EqualKey = Keyequal<T_Key>,
				typename Alloc = allocator<pair <const T_Key, T_Type> > 
			> 
	class T
		>
class Cache_Table: 
	public Table<T,Key<Operation>, Node >
{

public:
	Cache_Table();
	
};

template<>
Cache_Table<google::dense_hash_map>::Cache_Table()
	:Table<google::dense_hash_map,Key<Operation>, Node >(Key<Operation>(EMPTY_,NULL,NULL),Key<Operation>(DELETE_,NULL,NULL))
{{
	
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
Cache_Table<T>::Cache_Table()
{{

}}



#endif
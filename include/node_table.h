#ifndef NODE_TABLE_H_
#define NODE_TABLE_H_
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
class Node_Table:
	public Table<T,string, Node >
{
public:
	Node_Table();
	
};

template<>
Node_Table<google::dense_hash_map>::Node_Table()
	:Table<google::dense_hash_map,string, Node >("$Empty$","$Delete$")
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
Node_Table<T>::Node_Table()
{{
	//this->unique_table.set_empty_key(string());
}}


#endif
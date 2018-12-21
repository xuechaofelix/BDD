#ifndef BDD_HASH_MAP_H_
#define BDD_HASH_MAP_H_

template<
	class Key, 
	class Value, 
	class HashFunc, 
	class EqualKey,
	class _Alloc = allocator<std::pair<const Key, Value> >
	>
class BDDHashMap;

template<
	class Key, 
	class Value, 
	class HashFunc, 
	class EqualKey,
	class _Alloc = allocator<std::pair<const Key, Value> >
	>
class HashNode
{
public:
    Key    _key;
    Value  _value;
    BDDHashMap<Key,Value,HashFunc,EqualKey,_Alloc> * next;

    HashNode(Key key, Value value)
    {
        _key = key;
        _value = value;
        next = NULL;
    }
    ~HashNode()
    {

    }
    HashNode& operator=(const HashNode& node)
    {
        _key = node.key;
        _value = node.key;
        next = node.next;
        return *this;
    }
};

template<
	class Key, 
	class Value, 
	class HashFunc, 
	class EqualKey,
	class _Alloc
	>
class BDDHashMap
{
public:
    BDDHashMap(int size = 100);
    ~BDDHashMap();
    bool insert(const Key& key, const Value& value,size_t hash_code=-1);
    bool insert(std::pair<Key,Value> &p);
    bool erase(const Key& key);
    Value find(const Key& key,size_t hash_code=-1);
    Value operator [](const Key& key);

private:
    HashFunc hash;
    EqualKey equal;
    HashNode<Key, Value,HashFunc,EqualKey,_Alloc> ** arraylist;
    unsigned int _size;
    Value value = NULL;
};


template<
	class Key, 
	class Value, 
	class HashFunc, 
	class EqualKey,
	class _Alloc
	>
BDDHashMap<Key, Value, HashFunc, EqualKey,_Alloc>::BDDHashMap(int size) : _size(size)
{
    hash = HashFunc();
    equal = EqualKey();
    arraylist = new HashNode<Key, Value,HashFunc,EqualKey,_Alloc>*[_size];
    for (unsigned i = 0; i < _size; i++)
        arraylist[i] = NULL;
}



template<
	class Key, 
	class Value, 
	class HashFunc, 
	class EqualKey,
	class _Alloc
	>
BDDHashMap<Key, Value, HashFunc, EqualKey,_Alloc>::~BDDHashMap()
{
    for(unsigned i =0;i<_size;i++)
    {
    	HashNode<Key,Value,HashFunc,EqualKey,_Alloc> * tmp = arraylist[i];
    	if(tmp!=NULL)
    	{
    		if(tmp->next != NULL)
    		{
    			delete tmp->next;
    		}
    		delete tmp;
    	}
    }
    
    delete[] arraylist;
}


template<
	class Key, 
	class Value, 
	class HashFunc, 
	class EqualKey,
	class _Alloc
	>
bool BDDHashMap<Key, Value, HashFunc, EqualKey,_Alloc>::insert(const Key& key, const Value& value,size_t hash_code)
{
	size_t Hash_Code;
	if(hash_code == -1)
		Hash_Code = hash(key);
	else
		Hash_Code = hash_code;
    int index = Hash_Code%_size;
    if(arraylist[index] == NULL)
    {
    	HashNode<Key, Value,HashFunc,EqualKey,_Alloc> * node = 
    		new HashNode<Key, Value,HashFunc,EqualKey,_Alloc>
    			(key,value);
    	arraylist[index] = node;
    }
    else
    {
    	arraylist[index]->next->insert(key,value,Hash_Code/_size);
    }
    return true;
}

template<
	class Key, 
	class Value, 
	class HashFunc, 
	class EqualKey,
	class _Alloc
	>
bool BDDHashMap<Key, Value, HashFunc, EqualKey,_Alloc>::insert(std::pair<Key,Value> &p)
{{
	return this->insert(p.first,p.second);
}}

template<
	class Key, 
	class Value, 
	class HashFunc, 
	class EqualKey,
	class _Alloc
	>
bool BDDHashMap<Key, Value, HashFunc, EqualKey,_Alloc>::erase(const Key& key)
{
    
}

template<
	class Key, 
	class Value, 
	class HashFunc, 
	class EqualKey,
	class _Alloc
	>
Value BDDHashMap<Key, Value, HashFunc, EqualKey,_Alloc>::find(const Key& key,size_t hash_code)
{
	size_t Hash_Code;
	if(hash_code == -1)
		Hash_Code = hash(key);
	else
		Hash_Code = hash_code;
    unsigned  index = Hash_Code % _size;
    if (arraylist[index] == NULL)
        return NULL;
    else
    {
        HashNode<Key, Value, HashFunc, EqualKey> * node = arraylist[index];
        if(node -> next != NULL)
        {
        	return node->next->find(key,Hash_Code/_size);
        }
        else
        {
        	return node -> _value;
        }
    }
}

template<
	class Key, 
	class Value, 
	class HashFunc, 
	class EqualKey,
	class _Alloc
	>
Value BDDHashMap<Key, Value, HashFunc, EqualKey, _Alloc>::operator [](const Key& key)
{
    return find(key);
}


#endif
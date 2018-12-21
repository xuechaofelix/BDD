#ifndef BDD_H_
#define BDD_H_

#include <string>
#include <vector>
#include <list>
#include "cache_table.h"
#include "unique_table.h"
#include "node_table.h"
#include "node.h"
#include "util.h"
#include "satisfy_map.h"
#include "satisfy_result.h"

using namespace std;

template<
	template<	typename T_Key,
				typename T_Type,
				typename HashFcn = Keyhash<T_Key>,
				typename EqualKey = Keyequal<T_Key>,
				typename Alloc = allocator<pair <const T_Key, T_Type> > 
			> 
	class T
		>
class BDD
{
private:
	Cache_Table<T> * cache ;
	Unique_Table<T> * unique ;
	Node_Table<T> * nodes;

	Node * Const_0;
	Node * Const_1;
	int var_num;

	Node * ApplySpecialCase(Operation op,Node * u,Node * v);
	bool SetNotRelation(Node * v,Node * u);
	Node * AddNewVariable(int order);
	Node * Restrict_part(Node* f,int order,bool b);
	Node * EliminateSame_Low_High_Node(Node * f);
	list<vector<Satisfy_Result> > * Satisfy_All_part(Node* f,
					Satisfy_Map<T,list<vector<Satisfy_Result> > > & result);
	vector<Satisfy_Result> * Satisfy_part(Node* f,
					Satisfy_Map<T,vector<Satisfy_Result> > & result);

public:
	BDD();
	Node * Apply(Operation op,Node * u,Node * v);
	Node * GetNodeByName(string name);

	Node * Var(int i);

	Node * NOT(Node* f);
	Node * AND(Node* f1,Node * f2);
	Node * OR(Node* f1,Node* f2);
	Node * XOR(Node* f1,Node* f2);

	Node * Restrict(Node* f,int order,bool b);
	Node * Compose(Node* f1,int order,Node* f2);
	Node * Exists(Node* f,vector<int> I);
	Node * ForAll(Node* f,vector<int> I);
	Node * RelProd(Node* f1,Node* f2,vector<int> I);
	
	bool Equal(Node* f1,Node* f2);
	bool Eval(Node* f,vector<bool> a);

	vector<Satisfy_Result> * Satisfy(Node* f);
	list<vector<Satisfy_Result> > * Satisfy_All(Node* f);

	Node * Get_0_Const();
	Node * Get_1_Const();

	void GenerateDotFile(string fileName,Node * root);

	void ParseFile(string fileName);

};

// template<>
// BDD<google::dense_hash_map>::BDD()
// {{
// 	var_num = 0;
// 	this->Const_0 = new Node(false);
// 	this->Const_1 = new Node(true);
// 	this->SetNotRelation(this->Const_0,this->Const_1);
// 	this->cache = new Cache_Table<google::dense_hash_map>();
// 	this->unique = new Unique_Table<google::dense_hash_map>();
// 	this->nodes = new Node_Table<google::dense_hash_map>();
// }}
template<
	template<	typename T_Key,
				typename T_Type,
				typename HashFcn = Keyhash<T_Key>,
				typename EqualKey = Keyequal<T_Key>,
				typename Alloc = allocator<pair <const T_Key, T_Type> > 
			> 
	class T
		>
BDD<T>::BDD()
{{
	var_num = 0;
	this->Const_0 = new Node(false);
	this->Const_1 = new Node(true);
	this->SetNotRelation(this->Const_0,this->Const_1);
	this->cache = new Cache_Table<T>();
	this->unique = new Unique_Table<T>();
	this->nodes = new Node_Table<T>();
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
Node * BDD<T>::Apply(Operation op,Node * u,Node * v)
{{
	Key<Operation> * key = new Key<Operation>(op,u,v);
	Node * result = this->cache->get(*key);
	if(result != NULL)
	{
		//cout<<"Get same"<<endl;
		return result;
	}
	result = ApplySpecialCase(op,u,v);
	if(result != NULL)
	{
		return result;
	}
	int x_u = u->Get_Order();
	int x_v = v->Get_Order();
	int x_k = x_u<x_v?x_u:x_v;
	Node * w= NULL;
	Node * w1=NULL, * w0=NULL;
	Node * u_0 = u, * u_1 = u;
	Node * v_0 = v, * v_1 = v;
	if(x_u == x_k)
	{
		u_0 = u->Get_low_Node();
		u_1 = u->Get_high_Node();
	}
	if(x_v == x_k)
	{
		v_0 = v->Get_low_Node();
		v_1 = v->Get_high_Node();
	}
	w1 = Apply(op,u_1,v_1);
	//this->GenerateDotFile("w1.dot",w1);
	w0 = Apply(op,u_0,v_0);
	//this->GenerateDotFile("w0.dot",w0);
	Node * tmp;
	Key<int> * unique_key = new Key<int>(x_k,w0,w1);
	if(w1->Is_Equal(w0))
	{
		w = w1;
	}
	else if((tmp = this -> unique -> get(*unique_key))!=NULL)
	{
		w = tmp;
	}
	else
	{
		w = new Node(w0,w1,x_k);
		this->unique->add(* unique_key,w);
	}

	this->cache->add(*key,w);
	return w;

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
Node * BDD<T>::ApplySpecialCase(Operation op,Node * u,Node * v)
{{
	switch (op){
		case NOT_:
			return u->Get_not_Node();
		case AND_:
			if(u->Is_0_Node() || v -> Is_0_Node())
			{
				return this->Get_0_Const();
			}
			if(u->Is_1_Node())
			{
				return v;
			}
			if(v -> Is_1_Node())
			{
				return u;
			}
			if(v -> Is_Equal(u))
			{
				return v;
			}
			if(v->Is_not_Relation(u))
			{
				return this->Get_0_Const();
			}
		break;	
		case OR_:
			if(u->Is_1_Node() || v -> Is_1_Node())
			{
				return this->Get_1_Const();
			}
			if(u->Is_0_Node())
			{
				return v;
			}
			if(v -> Is_0_Node())
			{
				return u;
			}
			if(v -> Is_Equal(u))
			{
				return v;
			}
			if(v->Is_not_Relation(u))
			{
				return this->Get_1_Const();
			}
		break;
		case XOR_:
			if(u->Is_1_Node())
			{
				return v -> Get_not_Node();
			}
			if(v->Is_1_Node())
			{
				return u->Get_not_Node();
			}
			if(u->Is_0_Node())
			{
				return v;
			}
			if(v -> Is_0_Node())
			{
				return u;
			}
			if(v -> Is_Equal(u))
			{
				return this->Get_0_Const();
			}
			if(v->Is_not_Relation(u))
			{
				return this->Get_1_Const();
			}
			
		break;

	}
	return NULL;
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
Node * BDD<T>::AddNewVariable(int order)
{{
	Node * tmp = new Node(this->Const_0,this->Const_1,order);
	Key<int> * unique_key = new Key<int>(order,this->Const_0,this->Const_1);
	this->unique->add(* unique_key,tmp);
	return tmp;
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
Node * BDD<T>::GetNodeByName(string name)
{{
	return this->nodes->get(name);
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
Node * BDD<T>::Get_0_Const()
{{
	return this->Const_0;
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
Node * BDD<T>::Get_1_Const()
{{
	return this->Const_1;
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
bool BDD<T>::SetNotRelation(Node * v,Node * u)
{{
	v->Set_not_Node(u);
	u->Set_not_Node(v);
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
Node * BDD<T>::EliminateSame_Low_High_Node(Node * f)
{{
	if(f->Get_high_Node() == NULL || f->Get_low_Node() == NULL) return f;
	if(f->Get_low_Node() == f->Get_high_Node())
	{
		Node * result = this->EliminateSame_Low_High_Node(f->Get_high_Node());

		if(f->Can_Remove())
		{
			Key<int> * unique_key = new Key<int>(f->Get_Order(),f->Get_low_Node(),f->Get_high_Node());
			this->unique->remove(*unique_key);
			delete f;
		}
		return result;
	}
	else
	{
		Node * hi = this->EliminateSame_Low_High_Node(f->Get_high_Node());
		Node * lo = this->EliminateSame_Low_High_Node(f->Get_low_Node());
		f->Set_high_Node(hi);
		f->Set_low_Node(lo);
		return f;
	}
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
Node * BDD<T>::Var(int i)
{{
	this->var_num ++;
	return this->AddNewVariable(i);

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
Node * BDD<T>::NOT(Node* f)
{{
	return this->Apply(NOT_,f,NULL);

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
Node * BDD<T>::AND(Node* f1,Node * f2)
{{
	return this->Apply(AND_,f1,f2);
	
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
Node * BDD<T>::OR(Node* f1,Node* f2)
{{
	return this->Apply(OR_,f1,f2);
	
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
Node * BDD<T>::XOR(Node* f1,Node* f2)
{{
	return this->Apply(XOR_,f1,f2);
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
Node * BDD<T>::Restrict_part(Node* f,int order,bool b)
{{
	int f_order = f->Get_Order();
	if(f_order > order || f_order==0)
	{
		return f;
	}	
	else if(f_order == order)
	{
		Node * result = b ==true ? f->Get_high_Node():f->Get_low_Node();
		return result;
	}
	else 
	{
		Node * hi = this->Restrict_part(f->Get_high_Node(),order,b);
		Node * lo = this->Restrict_part(f->Get_low_Node(),order,b);
		Key<int> *unique_key = new Key<int>(f_order,lo,hi);
		Node * tmp;
		if((tmp = this -> unique -> get(*unique_key))!=NULL)
		{
			return tmp;
		}
		else
		{
			Node * result = f->clone();
			result->Set_high_Node(hi);
			result->Set_low_Node(lo);
			this->unique->add(*unique_key,result);
			return result;
		}
	}
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
Node * BDD<T>::Restrict(Node* f,int order,bool b)
{{
	Node * result = this->Restrict_part(f,order,b);
	return this->EliminateSame_Low_High_Node(result);
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
Node * BDD<T>::Compose(Node* f1,int order,Node* f2)
{{
	Node * f1_1 = this->Restrict(f1,order,true);
	Node * f1_0 = this->Restrict(f1,order,false);
	Node * f1_f2_1 = this->AND(f1_1,f2);
	Node * f1_notf2_0 = this->AND(f1_0,this->NOT(f2));
	return this->OR(f1_f2_1,f1_notf2_0);
	
	
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
Node * BDD<T>::Exists(Node* f,vector<int> I)
{{
	Node * result = f;
	int len = I.size();
	for(int i=0;i<len;i++)
	{
		int order = I[i];
		Node * tmp_true = this->Restrict(result,order,true);
		Node * tmp_false = this->Restrict(result,order,false);
		result = this->OR(this->OR(result,tmp_true),tmp_false);
	}
	return result;
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
Node * BDD<T>::ForAll(Node* f,vector<int> I)
{{
	Node * result = f;
	int len = I.size();
	for(int i=0;i<len;i++)
	{
		int order = I[i];
		Node * tmp_true = this->Restrict(result,order,true);
		Node * tmp_false = this->Restrict(result,order,false);
		result = this->AND(this->AND(result,tmp_true),tmp_false);
	}
	return result;
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
Node * BDD<T>::RelProd(Node* f1,Node* f2,vector<int> I)
{{
	Node * f1_and_f2 = this->AND(f1,f2);
	return this->Exists(f1_and_f2,I);
	
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
bool BDD<T>::Equal(Node* f1,Node* f2)
{{
	return f1->Is_Equal(f2);
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
bool BDD<T>::Eval(Node* f,vector<bool> a)
{{
	Node * result = f;
	int len = a.size();
	for(int i=0;i<len;i++)
	{
		bool b = a[i];
		result = b?result->Get_high_Node():result->Get_low_Node();
		if(result ==NULL)
		{
			cout<<"Evalue the assign wrong!!Please check the input"<<endl;
			break; 
		}
	}
	return result->Is_Equal(this->Const_1)?true:false;
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
vector<Satisfy_Result> * BDD<T>::Satisfy_part(Node* f,Satisfy_Map<T,vector<Satisfy_Result> > & result)
{{
	Key<int> * key = new Key<int>(f->Get_Order(),f->Get_low_Node(),f->Get_high_Node());
	vector<Satisfy_Result> * v;
	if((v = result.get(*key))!=NULL) return v;
	v = new vector<Satisfy_Result>();
	if(f->Is_1_Node()) 
	{
		//result.add(*key,v);
		return v;
	}
	if(f->Is_0_Node()) 
	{
		return NULL;
	}

	vector<Satisfy_Result> * hi = Satisfy_part(f->Get_high_Node(),result);
	if(hi!=NULL) 
	{
		v->insert(v->end(),hi->begin(),hi->end());
		v->push_back(Satisfy_Result(f->Get_Order(),true));
		result.add(*key,v);
		return v;
	}
	vector<Satisfy_Result> * lo = Satisfy_part(f->Get_low_Node(),result);
	if(lo!=NULL) 
	{
		v->insert(v->end(),lo->begin(),lo->end());
		v->push_back(Satisfy_Result(f->Get_Order(),false));
		result.add(*key,v);
		return v;
	}

	return NULL;
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
vector<Satisfy_Result> * BDD<T>::Satisfy(Node* f)
{{
	Satisfy_Map<T,vector<Satisfy_Result> >  result;
	return Satisfy_part(f,result);
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
list<vector<Satisfy_Result> > * BDD<T>::Satisfy_All_part(Node* f,
					Satisfy_Map<T,list<vector<Satisfy_Result> > > & result)
{{
	Key<int> * key = new Key<int>(f->Get_Order(),f->Get_low_Node(),f->Get_high_Node());
	list<vector<Satisfy_Result> > * l;
	if((l = result.get(*key))!=NULL) return l;
	l = new list<vector<Satisfy_Result> >();
	if(f->Is_1_Node()) 
	{
		vector<Satisfy_Result> v;
		l->push_back(v);
		//result.add(*key,l);
		return l;
	}
	if(f->Is_0_Node()) return NULL;


	list<vector<Satisfy_Result> > * hi = Satisfy_All_part(f->Get_high_Node(),result);
	
	list<vector<Satisfy_Result> > * lo = Satisfy_All_part(f->Get_low_Node(),result);

	if(hi!=NULL && lo == NULL)
	{
		l->insert(l->end(),hi->begin(),hi->end());
		list<vector<Satisfy_Result> >::iterator iter = l->begin();
		for(;iter != l->end();iter++)
		{
			(*iter).push_back(Satisfy_Result(f->Get_Order(),true));
		}
	}
	else if(hi == NULL && lo !=NULL)
	{
		l->insert(l->end(),lo->begin(),lo->end());
		list<vector<Satisfy_Result> >::iterator iter = l->begin();
		for(;iter != l->end();iter++)
		{
			(*iter).push_back(Satisfy_Result(f->Get_Order(),false));
		}
	}
	else if(hi != NULL && lo != NULL)
	{

		list<vector<Satisfy_Result> > tmp;
		tmp.insert(tmp.end(),hi->begin(),hi->end());
		list<vector<Satisfy_Result> >::iterator iter = tmp.begin();
		for(;iter != tmp.end();iter++)
		{
			(*iter).push_back(Satisfy_Result(f->Get_Order(),true));
		}
		l->insert(l->end(),lo->begin(),lo->end());
		iter = l->begin();
		for(;iter != l->end();iter++)
		{
			(*iter).push_back(Satisfy_Result(f->Get_Order(),false));
		}
		l->insert(l->end(),tmp.begin(),tmp.end());

	}
	else return NULL;

	result.add(*key,l);
	return l;
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
list<vector<Satisfy_Result> > *BDD<T>::Satisfy_All(Node* f)
{{
	Satisfy_Map<T,list<vector<Satisfy_Result> > > result;
	return Satisfy_All_part(f,result);
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
void BDD<T>::GenerateDotFile(string fileName,Node * root)
{{
	string content = "digraph Root_Graph{\n";
	content += "rankdir=TB;\n";
	set<Node * > found;
	content += root->GenerateDotCode(found);
	content += "}\n";
	Write2File(fileName,content);

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
void BDD<T>::ParseFile(string fileName)
{{
	ifstream in(fileName); 
	string line; 
	if(in)  
	{ 
		while (getline (in, line))  
		{ 
			size_t po = line.find("=",0);
			if(po != string::npos)
			{
				string name = line.substr(0,po);

			}

		} 
	} 
	else 
	{ 
		cout <<"no such file" << endl; 
	} 
	return 0;

}}






#endif
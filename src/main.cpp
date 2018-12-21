#include <ext/hash_map>
#include <unordered_map>
#include <google/dense_hash_map>
#include <string>
#include <time.h>

#include "bdd.h"
#include "cache_table.h"
#include "bdd_hash_map.h"

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
Node * N_Queens(int n,BDD<T> * b)
{
	Node * variable_[n][n];
	Node * result = b->Get_1_Const();
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			variable_[i][j] = b->Var(i*n+j);
		}
	}
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			Node * tmp_result = b->Get_1_Const();
			for(int l=0;l<n;l++)
			{
				if(l != j)
					tmp_result = b->AND(tmp_result,b->NOT(variable_[i][l]));
				if(l != i)
				{
					tmp_result = b->AND(tmp_result,b->NOT(variable_[l][j]));
					if(j+l-i >= 0 && j+l-i<n)
					{
						tmp_result = b->AND(tmp_result,b->NOT(variable_[l][j+l-i]));
					}
					if(j+i-l >= 0 && j+i-l<n)
					{
						tmp_result = b->AND(tmp_result,b->NOT(variable_[l][j+i-l]));
					}
				}
				
			}
			tmp_result = b->OR(tmp_result,b->NOT(variable_[i][j]));
			result = b->AND(result,tmp_result);
			//b->GenerateDotFile(toString<int>(i*n+j)+".dot",result);
		}
	}
	for(int i=0;i<n;i++)
	{
		Node * tmp_result = b->Get_0_Const();
		for(int j=0;j<n;j++)
		{
			tmp_result = b->OR(tmp_result,variable_[i][j]);
		}
		result = b->AND(result,tmp_result);
	}
	return result;
}
int main()
{
	BDD<__gnu_cxx::hash_map> * b = new BDD<__gnu_cxx::hash_map>();
	list<vector<Satisfy_Result> > * l;
	clock_t startTime,endTime;
	startTime = clock();
	Node * result = N_Queens(8,b);
	cout<<"Construct BDD Down!"<<endl;
	l = b->Satisfy_All(result);
	endTime = clock();
	cout<< "Totle Time : " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	b->GenerateDotFile("eight_queens.dot",result);
	cout<<l->size()<<endl;
	list<vector<Satisfy_Result> >::iterator iter = l->begin();
	for(;iter!=l->end();iter++)
	{
		vector<Satisfy_Result>::iterator iter1 = iter->begin();
		for(;iter1!=iter->end();iter1++)
		{
			cout<<*iter1<<" ";
		}
		cout<<endl;
	}
}
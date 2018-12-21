#include <ext/hash_map>
#include <unordered_map>
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
	Node * result = N_Queens(12,b);
	cout<<"Construct BDD Down!"<<endl;
	l = b->Satisfy_All(result);
	endTime = clock();
	cout<< "Totle Time : " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	b->GenerateDotFile("eight_queens.dot",result);
	cout<<l->size()<<endl;
	// list<vector<Satisfy_Result> >::iterator iter = l->begin();
	// for(;iter!=l->end();iter++)
	// {
	// 	vector<Satisfy_Result>::iterator iter1 = iter->begin();
	// 	for(;iter1!=iter->end();iter1++)
	// 	{
	// 		cout<<*iter1<<" ";
	// 	}
	// 	cout<<endl;
	// }

	// Node * const_0 = b->Get_0_Const();
	// Node * const_1 = b->Get_1_Const();
	// Node * f_1 = b->Var(1,"f_1");
	// Node * f_2 = b->Var(2,"f_2");
	// Node * f_3 = b->Var(3,"f_3");
	// Node * f_4 = b->Var(4,"f_4");
	// Node * f_5 = b->Var(5,"f_5");
	// Node * f_6 = b->Var(6,"f_6");
	// f_1->Show();
	// f_2->Show();
	// f_3->Show();
	// Node * result = b->OR(f_1,f_2,"f_4");
	// result = b->AND(result,f_3,"f_5");
	// result = b->NOT(result,"f_6");
	// result = b->AND(result,b->NOT(f_4));
	// result = b->AND(result,b->NOT(f_5));
	// result = b->OR(result,b->NOT(f_6));
	// //result->Set_high_Node(result->Get_low_Node());
	// // result = b->Restrict(result,2,false,"f_7");
	// // result->Show();
	// vector<Satisfy_Result> * l1;
	// //result = b->GetNodeByName("f_6");
	// l1 = b->Satisfy(result);
	// vector<Satisfy_Result>::iterator iter1 = l1->begin();
	// for(;iter1!=l1->end();iter1++)
	// {
	// 	cout<< *iter1<<" ";
	// }
	// cout<<endl;
	 
	// //result = b->GetNodeByName("f_6");
	
	// cout<<l->size()<<endl;
	// iter = l->begin();
	// for(;iter!=l->end();iter++)
	// {
	// 	vector<Satisfy_Result>::iterator iter1 = iter->begin();
	// 	for(;iter1!=iter->end();iter1++)
	// 	{
	// 		cout<<*iter1<<" ";
	// 	}
	// 	cout<<endl;
	// }
	//cout<<*b->Satisfy_All(result)<<endl;
	//cout<<l;
	// string fileName = "BDD.dot";
	// b->GenerateDotFile(fileName,result);
}
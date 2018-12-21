#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <iostream>
#include <set>
using namespace std;

class Node
{
private:
	Node * low;
	Node * high;
	Node * not_;
	int order;
	int reference;

	Node(int o);


public:
	Node(Node * lo, Node * hi, int o);
	Node(bool f);
	Node * clone();
	// Node * cloneAll();
	// bool operator ==(const Node & n) const;

	void IncreaseRef();
	void DecreaseRef();

	Node * Get_low_Node() const;
	Node * Get_high_Node() const;
	Node * Get_not_Node() const;
	int Get_Reference() const;
	int Get_Order() const;
	string Get_Dot_Name() const;

	void Set_low_Node(Node * lo);
	void Set_high_Node(Node * hi);
	void Set_not_Node(Node * n);

	bool Is_Equal(Node * n);
	bool Is_Leaves();
	bool Is_0_Node();
	bool Is_1_Node();
	bool Is_not_Relation(Node * n);

	bool Can_Remove();

	string GenerateDotCode(set<Node*> & found);

	~Node();
};

#endif
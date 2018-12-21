#include <string>
#include <iostream>
#include "node.h"
#include "util.h"

using namespace std;

Node::Node(Node * lo, Node * hi, int o)
{{
	this->reference = 0;
	this->low = NULL;
	this->high = NULL;
	this->not_ = NULL;
	this-> not_ = new Node(o);
	this-> not_ ->Set_not_Node(this);
	order = o;
	this->Set_high_Node(hi);
	this->Set_low_Node(lo);
	
}}
Node::Node(int o)
{{
	this->reference = 0;
	this->low = NULL;
	this->high = NULL;
	this->not_ = NULL;
	order = o;
}}
Node::Node(bool f)
{{
	low = NULL;
	high = NULL;
	if(f)
		order = -1;
	else
		order = -2;
	reference = 0;

}}

Node * Node::clone()
{{
	if(this->order <0)
	{
		return this;
	}
	else
	{
		Node * result = new Node(this->low,this->high,this->order);
		return result;
	}
}}
//TODO::
// Node * Node::cloneAll()
// {{
// 	if(this->order == -1)
// 	{
// 		Node * result = new Node(this->name);
// 		result->Set_not_Node(this->not_);
// 		return result;
// 	}
// 	else
// 	{
// 		Node * lo = this->low->cloneAll();
// 		Node * hi = this->high->cloneAll();
// 		Node * result = new Node(this->name,lo,hi,this->order);
// 		result -> Set_not_Node(this->not_);
// 		return result;
// 	}
// }}

// bool Node::operator ==(const Node & n) const
// {{
// 	// return this == &n;
// 	// if(this->order == -1)
// 	// {
// 	// 	if(n.Get_Order() != -1) return false;
// 	// 	return this->name == n.Get_Name();
// 	// }
// 	// return this->order == n.Get_Order()
// 	// 		&& this->low == n.Get_low_Node()
// 	// 		&& this->high == n.Get_high_Node();
// }}
bool Node::Is_Equal(Node * n)
{{
	return  (this-> Get_Order() == n->Get_Order())&&
			(this-> Get_high_Node() == n->Get_high_Node()) &&
			(this-> Get_low_Node() == n->Get_low_Node());
}}

bool Node::Is_not_Relation(Node * n)
{{
	if(this->not_ == NULL) return false;
	return this->not_->Is_Equal(n);
}}

bool Node::Is_Leaves()
{{
	return this->order<0;
}}

bool Node::Is_0_Node()
{{
	return this->order == -2;
}}
bool Node::Is_1_Node()
{{
	return this->order == -1;
}}

Node * Node::Get_not_Node() const
{{
	return this->not_;
}}

Node * Node::Get_low_Node() const
{{
	return this->low;
}}
Node * Node::Get_high_Node() const
{{
	return this->high;
}}

int Node::Get_Reference() const
{{
	return this->reference;
}}

int Node::Get_Order() const
{{
	return this -> order;
}}

void Node::Set_low_Node(Node * lo)
{{
	if(lo == this->low) return;
	// if(this->low!=NULL)
	// {
	// 	this->low->DecreaseRef();
	// }
	this->low = lo;
	this->not_->Set_low_Node(lo->Get_not_Node());
	// if(lo != NULL)
	// {
	// 	lo->IncreaseRef();
	// }
}}
void Node::Set_high_Node(Node * hi)
{{
	if(hi == this->high) return;
	if(this->high!=NULL)
	{
		this->high->DecreaseRef();
	}
	this->high = hi;
	this->not_->Set_high_Node(hi->Get_not_Node());
	if(hi !=NULL)
	{
		hi->IncreaseRef();
	}
}}
void Node::Set_not_Node(Node * n)
{{
	this->not_ = n;
}}

void Node::IncreaseRef()
{{
	this->reference ++;
}}

void Node::DecreaseRef()
{{
	this->reference --;
}}

bool Node::Can_Remove()
{{
	return this->reference ==0 && this->not_->Get_Reference() ==0;
}}

string Node::Get_Dot_Name() const
{{
	if(this->order>=0)
	{
		return  "\""+
				toString<void*>((void*)this->Get_low_Node()).substr(2)+
				"_"+
				toString<int>(this->order)+
				"_"+
				toString<void*>((void*)this->Get_high_Node()).substr(2)+
				"\"";
	}
	else if(this->order == -1) return "\"1\"";
	else if(this->order == -2) return "\"0\"";
	else return "";
}}
string Node::GenerateDotCode(set<Node *> & found)
{{
	string result = "";
	if(found.find(this) != found.end())
	{
		return result;
	}
	found.insert(this);
	string name = this->Get_Dot_Name();
	if(this->low == NULL && this->high ==NULL)
	{
		result += name +" [shape = \"record\", label = \""+name+"\"]\n";
	}
	else if(this->low == NULL && this->high != NULL)
	{
		result += name +" [label = \""+toString<int>(this->order)+"\"]\n";
		
		result += this->high -> GenerateDotCode(found);
		result += name + " -> " + this->high->Get_Dot_Name() + ";\n";
	}
	else if(this->low != NULL && this->high == NULL)
	{
		result += name +" [label = \""+toString<int>(this->order)+"\"]\n";
		
		result += this->low -> GenerateDotCode(found);
		result += name + " -> " + this->low->Get_Dot_Name() + "[style = \"dashed\"];\n";
	}
	else
	{
		result += this->Get_Dot_Name() +" [label = \""+toString<int>(this->order)+"\"]\n";
		
		result += this->low -> GenerateDotCode(found);
		result += this->high -> GenerateDotCode(found);
		result += this->Get_Dot_Name() + " -> " + this->high->Get_Dot_Name() + ";\n";	
		result += this->Get_Dot_Name() + " -> " + this->low->Get_Dot_Name() + "[style = \"dashed\"];\n";	
	}
	return result;
}}

Node::~Node()
{
	if(this->low !=NULL)
	{
		this->low->DecreaseRef();
	}
	if(this->high !=NULL)
	{
		this->high->DecreaseRef();
	}
	if(this->not_!=NULL)
	{
		if(this->not_->Get_low_Node() !=NULL)
		{
			this->not_->Get_low_Node()->DecreaseRef();
		}
		if(this->not_->Get_high_Node() !=NULL)
		{
			this->not_->Get_high_Node()->DecreaseRef();
		}
	}
}
#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

bool Write2File(string fileName,string content);

template<typename T> 
string toString(const T& t){
    ostringstream oss;  
    oss<<t;             
    return oss.str();  
}



#endif
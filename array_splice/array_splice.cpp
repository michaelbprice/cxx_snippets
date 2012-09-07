#include <cstddef>
#include <tuple>
#include <array>
#include <iostream>

#include "array_splice.h"
 
using namespace std;
 
int main ()
{
    array<int, 10> a = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    
    for (auto e : a) { cout << e << " "; } cout << endl << endl;
 
    //auto t0 = array_splice<9>(a);
    //auto t1= array_splice<2, 3, 4>(a);
    //auto t2= array_splice<2, 3, 4, 5>(a);
    
    //auto t = array_splice<2, 3, 2, 2, 1>(a);
    //auto t = array_splice<10>(a);
    //auto t = array_splice<1,1,1,1,1,1,1,1,1,1>(a);
    auto t = array_splice<0, 5, 0 , 5, 0>(a);
 
    cout << get<0>(t).size() << ": ";
    for (auto e : get<0>(t)) { cout << e << " "; } cout << endl;
 
    cout << get<1>(t).size() << ": ";
    for (auto e : get<1>(t)) { cout << e << " "; } cout << endl;
 
    cout << get<2>(t).size() << ": ";
    for (auto e : get<2>(t)) { cout << e << " "; } cout << endl;
 
    cout << get<3>(t).size() << ": ";
    for (auto e : get<3>(t)) { cout << e << " "; } cout << endl;
 
    cout << get<4>(t).size() << ": ";
    for (auto e : get<4>(t)) { cout << e << " "; } cout << endl << endl;
 
    cout << "Done" << endl;
 
    return 0;
}

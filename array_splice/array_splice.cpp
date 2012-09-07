#include <cstddef>
#include <tuple>
#include <array>
#include <iostream>
 
using namespace std;
 
template <size_t ... All>
struct const_sum { };
 
template <size_t Value, size_t ... Rest>
struct const_sum<Value, Rest...>
{
    static const size_t value = Value + const_sum<Rest...>::value;
};
 
template <size_t Value>
struct const_sum<Value>
{
    static const size_t value = Value;
};
 
 
template <size_t ... Slices, typename T, size_t N>
std::tuple<std::array<T, Slices>...> array_splice (const std::array<T, N> & a)
{
    static_assert(const_sum<Slices...>::value == N, "Does not compute");
 
    std::tuple<std::array<T, Slices>...> t;
 
 
    return t;
}
 
int main ()
{
    std::array<int, 10> a = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    
    for (int * e = a.begin(); e != a.end(); ++e) { cout << *e << " "; } cout << endl << endl;
 
    std::tuple<std::array<int,2>,
               std::array<int,3>,
               std::array<int,2>,
               std::array<int,2>,
               std::array<int,1>> t = array_splice<2, 3, 2, 2, 1>(a);
 
    cout << std::get<0>(t).size() << ": ";
    for (int * e = std::get<0>(t).begin(); e != std::get<0>(t).end(); ++e)
    { cout << *e << " "; } cout << endl;
 
    cout << std::get<1>(t).size() << ": ";
    for (int * e = std::get<1>(t).begin(); e != std::get<1>(t).end(); ++e)
    { cout << *e << " "; } cout << endl;
 
    cout << std::get<2>(t).size() << ": ";
    for (int * e = std::get<2>(t).begin(); e != std::get<2>(t).end(); ++e)
    { cout << *e << " "; } cout << endl;
 
    cout << std::get<3>(t).size() << ": ";
    for (int * e = std::get<3>(t).begin(); e != std::get<3>(t).end(); ++e)
    { cout << *e << " "; } cout << endl;
 
    cout << std::get<4>(t).size() << ": ";
    for (int * e = std::get<4>(t).begin(); e != std::get<4>(t).end(); ++e)
    { cout << *e << " "; } cout << endl << endl;
 
    cout << "Done" << endl;
 
    return 0;
}
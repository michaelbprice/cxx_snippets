#include <cstddef>
#include <tuple>
#include <array>
#include <iostream>
 
using namespace std;
 
template <size_t Value, size_t ... Rest>
struct const_sum
{
    static constexpr size_t value = Value + const_sum<Rest...>::value;
};
 
template <size_t Value>
struct const_sum<Value>
{
    static constexpr size_t value = Value;
};

template <size_t IndicesToGo, size_t Value, size_t ... Rest>
struct variadic_index
{
    static constexpr size_t value = variadic_index<IndicesToGo - 1, Rest...>::value;
};

template <size_t Value, size_t ... Rest>
struct variadic_index<0, Value, Rest...>
{
    static constexpr size_t value = Value;
};
 

template <size_t TupleIndex, size_t StartOffset, size_t IndicesLeft>
struct array_to_tuple
{
    template <size_t ... Slices, typename T, size_t N>
    static void copy (std::tuple<std::array<T, Slices>...> & t, const std::array<T, N> & a)
    {
        auto & dest = std::get<TupleIndex>(t);

        constexpr size_t Left = StartOffset;
        constexpr size_t Right = StartOffset + variadic_index<TupleIndex, Slices...>::value;

        std::copy(a.begin() + Left, a.begin() + Right, dest.begin());

        array_to_tuple<TupleIndex + 1, Right, IndicesLeft - 1>::copy(t, a);
    }
};

template <size_t TupleIndex, size_t StartOffset>
struct array_to_tuple<TupleIndex, StartOffset, 1>
{
    template <size_t ... Slices, typename T, size_t N>
    static void copy (std::tuple<std::array<T, Slices>...> & t, const std::array<T, N> & a)
    {
        auto & dest = std::get<TupleIndex>(t);

        constexpr size_t Left = StartOffset;
        constexpr size_t Right = StartOffset + variadic_index<TupleIndex, Slices...>::value;

        std::copy(a.begin() + Left, a.begin() + Right, dest.begin());
    }
};

 
template <size_t ... Slices, typename T, size_t N>
std::tuple<std::array<T, Slices>...> array_splice (const std::array<T, N> & a)
{
    static_assert(const_sum<Slices...>::value >= N, "You did not provide enough splice arguments!");
    static_assert(const_sum<Slices...>::value <= N, "You provided too many splice arguments!");
 
    std::tuple<std::array<T, Slices>...> t;
 
    constexpr size_t splice_count = sizeof...(Slices);
 
    array_to_tuple<0, 0, splice_count>::copy(t, a);

    return t;
}
 
int main ()
{
    std::array<int, 10> a = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    
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

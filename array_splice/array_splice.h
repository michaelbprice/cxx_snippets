#include <cstddef>
#include <tuple>
#include <array>
#include <algorithm>

#include "../static_arithmetic/static_arithmetic.h"
 
 
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
    static_assert(static_arithmetic<Slices...>::sum >= N, "You did not provide enough splice arguments!");
    static_assert(static_arithmetic<Slices...>::sum <= N, "You provided too many splice arguments!");
 
    std::tuple<std::array<T, Slices>...> t;
 
    constexpr size_t splice_count = sizeof...(Slices);
 
    array_to_tuple<0, 0, splice_count>::copy(t, a);

    return t;
}
 

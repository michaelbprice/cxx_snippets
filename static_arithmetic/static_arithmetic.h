#include <cstddef>
 
template <size_t Value, size_t ... Rest>
struct static_arithmetic
{
    static constexpr size_t count = 1 + sizeof...(Rest);
    static constexpr size_t sum = Value + static_arithmetic<Rest...>::sum;
    static constexpr size_t product = Value * static_arithmetic<Rest...>::product;
    static constexpr size_t int_median = sum / count;
};
 
template <size_t Value>
struct static_arithmetic<Value>
{
    static constexpr size_t count = 1;
    static constexpr size_t sum = Value;
    static constexpr size_t product = Value;
    static constexpr size_t int_median = Value;
};


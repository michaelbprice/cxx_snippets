#include <cstddef>
 
template <size_t Value, size_t ... Rest>
struct static_arithmetic
{
    static constexpr size_t sum = Value + static_arithmetic<Rest...>::sum;
};
 
template <size_t Value>
struct static_arithmetic<Value>
{
    static constexpr size_t sum = Value;
};


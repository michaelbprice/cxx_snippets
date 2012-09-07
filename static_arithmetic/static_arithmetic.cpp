#include "static_arithmetic.h"

int main ()
{
    static_assert(static_arithmetic<0, 1, 2, 3, 4>::count == 5, "'count' member not correct");
    static_assert(static_arithmetic<0, 1, 2, 3, 4>::sum == 10, "'sum' member not correct");
    static_assert(static_arithmetic<0, 1, 2, 3, 4>::product == 0, "'product' member not correct");
    static_assert(static_arithmetic<0, 1, 2, 3, 4>::int_median == 2, "'int_median' member not correct");

    static_assert(static_arithmetic<1, 2, 3, 4, 5>::count == 5, "'count' member not correct");
    static_assert(static_arithmetic<1, 2, 3, 4, 5>::sum == 15, "'sum' member not correct");
    static_assert(static_arithmetic<1, 2, 3, 4, 5>::product == 120, "'product' member not correct");
    static_assert(static_arithmetic<1, 2, 3, 4, 5>::int_median == 3, "'int_median' member not correct");

    static_assert(static_arithmetic<5>::count == 1, "'count' member not correct");
    static_assert(static_arithmetic<5>::sum == 5, "'sum' member not correct");
    static_assert(static_arithmetic<5>::product == 5, "'product' member not correct");
    static_assert(static_arithmetic<5>::int_median == 5, "'int_median' member not correct");

    return 0;
}


#include <stdio.h>
#include <stdarg.h>
#include "var_arg_util.h"

int add(int count, ...)
{
    int total = 0;
    va_list va;
    printf("Count is %d and sum is:");

    va_start(va, count);   // Start with arguments after "count"

    for (int i = 0; i < count; i++) {
        int n = va_arg(va, int);   // Get the next int

        total += n;
    }

    va_end(va);  // All done

    return total;
}
// int add_nc()
// {
//     int total = 0;
//     va_list va;
//     printf("Count is %d and sum is:");

//     va_start(va, count);   // Start with arguments after "count"

//     for (int i = 0; i < count; i++) {
//         int n = va_arg(va, int);   // Get the next int

//         total += n;
//     }

//     va_end(va);  // All done

//     return total;
// }
int mainoff(void)
{
    printf("%d\n", add(4, 6, 2, -4, 17));  // 6 + 2 - 4 + 17 = 21
    printf("%d\n", add(2, 22, 44));        // 22 + 44 = 66
    printf("%d\n", my_add(2, 22, 44));        // 22 + 44 = 66

}
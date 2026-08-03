/**The configuration item
 * 
 * */

/**The Usage
 * #define STB_atoi
 * #include "atoi.h"
 * */
#ifndef ATOI_H
#define ATOI_H 

int alibatoi(const char *s);

#ifdef STB_atoi

#include <limits.h>
#include <stdbool.h>

int alibatoi(const char *s) 
{
    int result = 0;
    bool negative = false;

    // 跳过前导空白(可选，提高健壮性)
    // Skip leading white space (optional, improve robustness)
    while (*s == ' ' || *s == '\t' || *s == '\n') s++;

    // 处理符号
    // Handle positive and negative symbols
    if (*s == '-') {
        negative = true;
        s++;
    } else if (*s == '+') {
        s++;
    }

    // 核心转换:result = result * 10+读取的每个数字的位数。
    // Core transformation: result = result * 10+digit for each number read.
    while (*s >= '0' && *s <= '9') {
        int digit = *s - '0';

        // Optional overflow check (based on INT_MAX/10)
        if (result > INT_MAX / 10 || (result == INT_MAX / 10 && digit > 7)) {
            return negative ? INT_MIN : INT_MAX;
        }

        result = result * 10 + digit;
        s++;
    }

    return negative ? -result : result;
}

#endif /* STB_atoi */
#endif /* ATOI_H */


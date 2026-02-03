/**The configuration item
 * #define CUSTOM_INT_BITS    15   // 整数位数 m
 * #define CUSTOM_FRAC_BITS   16   // 小数位数 n
 * */

/**The Usage
 * #define STB_fixed
 * #include "fixed.h"
 * */

#ifndef _FIXED_H_
#define _FIXED_H_ 

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

// 自定义定点数类型: Qm.n
#ifndef CUSTOM_INT_BITS
    #define CUSTOM_INT_BITS    15   // 整数位数 m
#endif

#ifndef CUSTOM_FRAC_BITS
    #define CUSTOM_FRAC_BITS   16   // 小数位数 n
#endif

#define CUSTOM_TOTAL_BITS  (CUSTOM_INT_BITS + CUSTOM_FRAC_BITS)

// 选择合适的基础类型
#if CUSTOM_TOTAL_BITS <= 8
typedef int8_t q_custom_t;
#elif CUSTOM_TOTAL_BITS <= 16
typedef int16_t q_custom_t;
#elif CUSTOM_TOTAL_BITS <= 32
typedef int32_t q_custom_t;
#else
typedef int64_t q_custom_t;
#endif

// ==================== 通用宏 ====================
// 计算缩放因子
#define Q_SCALE(frac_bits)        ((1U) << (frac_bits))

// 计算最大值（有符号，考虑一位符号位）
#define Q_MAX_VALUE(int_bits, frac_bits) \
    ((1LL << ((int_bits) + (frac_bits) - 1)) - 1)

// 计算最小值（有符号，考虑一位符号位）
#define Q_MIN_VALUE(int_bits, frac_bits) \
    (-(1LL << ((int_bits) + (frac_bits) - 1)))

// 浮点数转定点数（四舍五入）
#define FLOAT_TO_Q(f, frac_bits, round) \
    ((f) >= 0 ? \
     ((f) * (1 << (frac_bits)) + (round ? 0.5f : 0.0f)) : \
     ((f) * (1 << (frac_bits)) - (round ? 0.5f : 0.0f)))

// 浮点数转定点数（无符号，四舍五入）
#define UFLOAT_TO_Q(f, frac_bits, round) \
    ((f) * (1 << (frac_bits)) + (round ? 0.5f : 0.0f))

// 定点数转浮点数
#define Q_TO_FLOAT(q, frac_bits) \
    ((float)(q) / (1 << (frac_bits)))

// 整数转定点数
#define INT_TO_Q(i, frac_bits) \
    ((i) << (frac_bits))

// 定点数取整数部分
#define Q_TO_INT(q, frac_bits) \
    ((q) >> (frac_bits))

// 定点数取小数部分
#define Q_FRAC_PART(q, frac_bits) \
    ((q) & ((1 << (frac_bits)) - 1))

// 定点数乘法（四舍五入）
#define Q_MUL(a, b, frac_bits) \
    ((int)(((int64_t)(a) * (b) + (1 << ((frac_bits) - 1))) >> (frac_bits)))

// 定点数除法（四舍五入）
#define Q_DIV(a, b, frac_bits) \
    ((int)(((int64_t)(a) << (frac_bits)) / (b)))

// 定点数格式转换（四舍五入）
#define Q_CONVERT(q, from_frac, to_frac) \
    ((from_frac) >= (to_frac) ? \
     ((q) + (1 << ((from_frac) - (to_frac) - 1))) >> ((from_frac) - (to_frac)) : \
     (q) << ((to_frac) - (from_frac)))


// ==================== 自定义 Qm.n 定点数 ====================
#if USE_CUSTOM_Q
// 自定义定点数常量
#define CUSTOM_SCALE      (1U << CUSTOM_FRAC_BITS)
#define CUSTOM_MAX        ((1LL << (CUSTOM_INT_BITS + CUSTOM_FRAC_BITS - 1)) - 1)
#define CUSTOM_MIN        (-(1LL << (CUSTOM_INT_BITS + CUSTOM_FRAC_BITS - 1)))
#define CUSTOM_MAX_FLOAT  ((float)CUSTOM_MAX / CUSTOM_SCALE)
#define CUSTOM_MIN_FLOAT  ((float)CUSTOM_MIN / CUSTOM_SCALE)

// 自定义定点数掩码
#define CUSTOM_INT_MASK   (((1ULL << CUSTOM_INT_BITS) - 1) << CUSTOM_FRAC_BITS)
#define CUSTOM_FRAC_MASK  ((1ULL << CUSTOM_FRAC_BITS) - 1)

// 自定义定点数函数
static inline q_custom_t q_custom_from_float(float f) {
    return (q_custom_t)FLOAT_TO_Q(f, CUSTOM_FRAC_BITS, true);
}

static inline q_custom_t q_custom_from_float_trunc(float f) {
    return (q_custom_t)FLOAT_TO_Q(f, CUSTOM_FRAC_BITS, false);
}

static inline float q_custom_to_float(q_custom_t q) {
    return Q_TO_FLOAT(q, CUSTOM_FRAC_BITS);
}

static inline q_custom_t q_custom_from_int(int i) {
    return (q_custom_t)INT_TO_Q(i, CUSTOM_FRAC_BITS);
}

static inline int q_custom_to_int(q_custom_t q) {
    return (int)Q_TO_INT(q, CUSTOM_FRAC_BITS);
}

static inline q_custom_t q_custom_frac_part(q_custom_t q) {
    return (q_custom_t)Q_FRAC_PART(q, CUSTOM_FRAC_BITS);
}

// 自定义定点数算术运算
static inline q_custom_t q_custom_add(q_custom_t a, q_custom_t b) { return a + b; }
static inline q_custom_t q_custom_sub(q_custom_t a, q_custom_t b) { return a - b; }

static inline q_custom_t q_custom_mul(q_custom_t a, q_custom_t b) {
    return (q_custom_t)Q_MUL(a, b, CUSTOM_FRAC_BITS);
}

static inline q_custom_t q_custom_div(q_custom_t a, q_custom_t b) {
    return (q_custom_t)Q_DIV(a, b, CUSTOM_FRAC_BITS);
}
#endif

#endif /* _FIXED_H_  */

#ifdef STB_fixed_TEST
// ==================== 测试代码 ====================

// 打印配置信息
void test_configuration(void) {
    printf("\n=== 自定义定点数配置信息测试 ===\n");
    printf("整数位数 (m): %d bits\n", CUSTOM_INT_BITS);
    printf("小数位数 (n): %d bits\n", CUSTOM_FRAC_BITS);
    printf("总位数: %d bits\n", CUSTOM_TOTAL_BITS);
    printf("基础数据类型: %s\n",
           CUSTOM_TOTAL_BITS <= 8 ? "int8_t" :
           CUSTOM_TOTAL_BITS <= 16 ? "int16_t" :
           CUSTOM_TOTAL_BITS <= 32 ? "int32_t" : "int64_t");
    printf("缩放因子: %u\n", CUSTOM_SCALE);
    printf("精度: 1/%.0f\n", (float)CUSTOM_SCALE);
    printf("分辨率: %f\n", 1.0f / CUSTOM_SCALE);
    printf("最大值 (整数): %lld\n", CUSTOM_MAX);
    printf("最小值 (整数): %lld\n", CUSTOM_MIN);
    printf("最大值 (浮点): %f\n", CUSTOM_MAX_FLOAT);
    printf("最小值 (浮点): %f\n", CUSTOM_MIN_FLOAT);
    printf("整数位掩码: 0x%llX\n", (unsigned long long)CUSTOM_INT_MASK);
    printf("小数位掩码: 0x%llX\n", (unsigned long long)CUSTOM_FRAC_MASK);

    // 测试范围
    printf("\n数值范围测试:\n");
    float test_float = 123.456f;
    q_custom_t test_q = q_custom_from_float(test_float);
    printf("浮点数 %f -> 定点数: %d (0x%X)\n",
           test_float, test_q, (unsigned int)test_q);
    printf("定点数 %d -> 浮点数: %f\n",
           test_q, q_custom_to_float(test_q));

    // 测试整数和小数部分
    int int_part = q_custom_to_int(test_q);
    q_custom_t frac_part = q_custom_frac_part(test_q);
    printf("整数部分: %d (0x%X)\n", int_part, int_part);
    printf("小数部分: %d (0x%X) -> 浮点: %f\n",
           frac_part, frac_part, (float)frac_part / CUSTOM_SCALE);
}

// 转换测试
void test_conversion(void) {
    printf("\n=== 转换精度测试 ===\n");

    // 浮点数到定点数转换测试
    float test_values[] = {0.0f, 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f, 5.0f};
    int num_tests = sizeof(test_values) / sizeof(test_values[0]);

    printf("浮点数 -> 定点数 (四舍五入) 转换测试:\n");
    printf("%-10s %-15s %-10s %-10s %-10s\n",
           "浮点数", "定点数(十六)", "定点数(十)", "转回浮点", "误差");

    for (int i = 0; i < num_tests; i++) {
        q_custom_t q_val = q_custom_from_float(test_values[i]);
        float recovered = q_custom_to_float(q_val);
        float error = fabsf(test_values[i] - recovered);

        printf("%-10.4f 0x%-13X %-10d %-10.4f %-10.6f\n",
               test_values[i], (unsigned int)q_val, q_val, recovered, error);
    }

    // 截断模式测试
    printf("\n浮点数 -> 定点数 (截断) 转换测试:\n");
    printf("%-10s %-15s %-10s %-10s %-10s\n",
           "浮点数", "定点数(十六)", "定点数(十)", "转回浮点", "误差");

    for (int i = 0; i < num_tests; i++) {
        q_custom_t q_val = q_custom_from_float_trunc(test_values[i]);
        float recovered = q_custom_to_float(q_val);
        float error = fabsf(test_values[i] - recovered);

        printf("%-10.4f 0x%-13X %-10d %-10.4f %-10.6f\n",
               test_values[i], (unsigned int)q_val, q_val, recovered, error);
    }

    // 整数转换测试
    printf("\n整数 -> 定点数转换测试:\n");
    int int_test[] = {0, 1, 2, 5, 10, 100, -1, -2, -5, -10, -100};
    int num_int_tests = sizeof(int_test) / sizeof(int_test[0]);

    for (int i = 0; i < num_int_tests; i++) {
        q_custom_t q_val = q_custom_from_int(int_test[i]);
        int recovered_int = q_custom_to_int(q_val);
        printf("整数 %4d -> 定点数: %5d -> 转回整数: %4d %s\n",
               int_test[i], q_val, recovered_int,
               (int_test[i] == recovered_int) ? "✓" : "✗");
    }
}

// 精度测试
void test_precision(void) {

    printf("\n=== 精度与误差测试 ===\n");
    printf("\33[32m""\n=== 精度与误差测试 ===\n""\33[0m");

    // 测试分辨率
    printf("分辨率测试:\n");
    printf("缩放因子: %u\n", CUSTOM_SCALE);
    printf("理论最小步长: %f\n", 1.0f / CUSTOM_SCALE);
    printf("实际最小可表示浮点值: %.10f\n", 1.0f / CUSTOM_SCALE);

    // 测试可表示的最大小数精度
    float max_fraction = 1.0f - 1.0f / CUSTOM_SCALE;
    printf("最大小数部分: %.4f (1 - 1/%d)\n", max_fraction, CUSTOM_SCALE);

    // 测试边界条件
    printf("\n边界条件测试:\n");
    float boundary_values[] = {
        0.0f,
        0.0625f,  // 1/16
        0.9999f,  // 接近1
        1.0f,
        127.9375f,  // 最大正数
        -0.0625f,   // 1/16
        -128.0f,    // 最小负数
    };

    for (int i = 0; i < sizeof(boundary_values) / sizeof(boundary_values[0]); i++) {
        if (boundary_values[i] > CUSTOM_MAX_FLOAT ||
                boundary_values[i] < CUSTOM_MIN_FLOAT) {
            printf("值 %.4f 超出范围 [%.4f, %.4f] -> 跳过\n",
                   boundary_values[i], CUSTOM_MIN_FLOAT, CUSTOM_MAX_FLOAT);
            continue;
        }

        q_custom_t q_val = q_custom_from_float(boundary_values[i]);
        float recovered = q_custom_to_float(q_val);
        float error = fabsf(boundary_values[i] - recovered);

        printf("值: %8.4f -> 定点: 0x%08X -> 浮点: %8.4f, 误差: %.6f\n",
               boundary_values[i], (unsigned int)q_val, recovered, error);
    }
}

// 算术运算测试
void test_arithmetic(void) {
    printf("\n=== 算术运算测试 ===\n");

    // 加减法测试
    printf("\n1. 加法测试:\n");
    float a_f = 3.5f, b_f = 2.25f;
    q_custom_t a_q = q_custom_from_float(a_f);
    q_custom_t b_q = q_custom_from_float(b_f);
    q_custom_t sum_q = q_custom_add(a_q, b_q);
    float sum_f = q_custom_to_float(sum_q);
    float expected_sum = a_f + b_f;

    printf("浮点: %.2f + %.2f = %.2f\n", a_f, b_f, expected_sum);
    printf("定点: 0x%X + 0x%X = 0x%X\n", (unsigned int)a_q, (unsigned int)b_q, (unsigned int)sum_q);
    printf("定点转浮点: %.2f + %.2f = %.2f\n",
           q_custom_to_float(a_q), q_custom_to_float(b_q), sum_f);
    printf("误差: %.6f\n", fabsf(expected_sum - sum_f));

    printf("\n2. 减法测试:\n");
    q_custom_t diff_q = q_custom_sub(a_q, b_q);
    float diff_f = q_custom_to_float(diff_q);
    float expected_diff = a_f - b_f;

    printf("浮点: %.2f - %.2f = %.2f\n", a_f, b_f, expected_diff);
    printf("定点: 0x%X - 0x%X = 0x%X\n", (unsigned int)a_q, (unsigned int)b_q, (unsigned int)diff_q);
    printf("定点转浮点: %.2f - %.2f = %.2f\n",
           q_custom_to_float(a_q), q_custom_to_float(b_q), diff_f);
    printf("误差: %.6f\n", fabsf(expected_diff - diff_f));

    printf("\n3. 乘法测试:\n");
    q_custom_t mul_q = q_custom_mul(a_q, b_q);
    float mul_f = q_custom_to_float(mul_q);
    float expected_mul = a_f * b_f;

    printf("浮点: %.2f * %.2f = %.2f\n", a_f, b_f, expected_mul);
    printf("定点: 0x%X * 0x%X = 0x%X\n", (unsigned int)a_q, (unsigned int)b_q, (unsigned int)mul_q);
    printf("定点转浮点: %.2f * %.2f = %.2f\n",
           q_custom_to_float(a_q), q_custom_to_float(b_q), mul_f);
    printf("误差: %.6f\n", fabsf(expected_mul - mul_f));

    printf("\n4. 除法测试:\n");
    if (b_q != 0) {
        q_custom_t div_q = q_custom_div(a_q, b_q);
        float div_f = q_custom_to_float(div_q);
        float expected_div = a_f / b_f;

        printf("浮点: %.2f / %.2f = %.4f\n", a_f, b_f, expected_div);
        printf("定点: 0x%X / 0x%X = 0x%X\n", (unsigned int)a_q, (unsigned int)b_q, (unsigned int)div_q);
        printf("定点转浮点: %.2f / %.2f = %.4f\n",
               q_custom_to_float(a_q), q_custom_to_float(b_q), div_f);
        printf("误差: %.6f\n", fabsf(expected_div - div_f));
    }

    printf("\n5. 复杂运算测试:\n");
    float c_f = 1.75f, d_f = 0.625f, e_f = 4.125f;
    q_custom_t c_q = q_custom_from_float(c_f);
    q_custom_t d_q = q_custom_from_float(d_f);
    q_custom_t e_q = q_custom_from_float(e_f);

    // 计算: (a + b) * c / d - e
    float result_f = (a_f + b_f) * c_f / d_f - e_f;
    q_custom_t result_q = q_custom_div(
                              q_custom_mul(
                                  q_custom_add(a_q, b_q),
                                  c_q
                              ),
                              d_q
                          );
    result_q = q_custom_sub(result_q, e_q);
    float result_q_f = q_custom_to_float(result_q);

    printf("表达式: (%.2f + %.2f) * %.2f / %.2f - %.2f\n", a_f, b_f, c_f, d_f, e_f);
    printf("浮点结果: %.4f\n", result_f);
    printf("定点结果: 0x%X -> %.4f\n", (unsigned int)result_q, result_q_f);
    printf("误差: %.6f\n", fabsf(result_f - result_q_f));

    // 溢出测试
    printf("\n6. 溢出测试:\n");
    float large1 = 100.0f;
    float large2 = 30.0f;
    q_custom_t large1_q = q_custom_from_float(large1);
    q_custom_t large2_q = q_custom_from_float(large2);

    q_custom_t mul_large = q_custom_mul(large1_q, large2_q);
    float mul_large_f = q_custom_to_float(mul_large);
    float expected_mul_large = large1 * large2;

    printf("大数乘法: %.2f * %.2f = %.2f\n", large1, large2, expected_mul_large);
    printf("定点结果: 0x%X -> %.2f\n", (unsigned int)mul_large, mul_large_f);
    printf("是否溢出: %s\n", fabsf(expected_mul_large - mul_large_f) > 1.0f ? "可能" : "否");
}

// 格式转换测试
void test_format_conversion(void) {
    printf("\n=== 格式转换测试 ===\n");

    // 从不同精度转换
    printf("1. 从 Q4.4 转换到 Q12.4:\n");
    int q4_4 = 0x53;  // 5.1875 in Q4.4
    q_custom_t converted_q = Q_CONVERT(q4_4, 4, CUSTOM_FRAC_BITS);
    printf("Q4.4: 0x%X (%f) -> Q%d.%d: 0x%X (%f)\n",
           q4_4, (float)q4_4 / 16.0f,
           CUSTOM_INT_BITS, CUSTOM_FRAC_BITS,
           (unsigned int)converted_q, q_custom_to_float(converted_q));

    printf("\n2. 从 Q8.8 转换到 Q12.4:\n");
    int q8_8 = 0x0123;  // 1.13671875 in Q8.8
    converted_q = Q_CONVERT(q8_8, 8, CUSTOM_FRAC_BITS);
    printf("Q8.8: 0x%X (%f) -> Q%d.%d: 0x%X (%f)\n",
           q8_8, (float)q8_8 / 256.0f,
           CUSTOM_INT_BITS, CUSTOM_FRAC_BITS,
           (unsigned int)converted_q, q_custom_to_float(converted_q));
}

// 性能测试
void test_performance(void) {
    printf("\n=== 性能基准测试 ===\n");

    const int NUM_ITERATIONS = 10000;

    // 浮点运算基准
    float float_result = 1.0f;
    float float_ops[NUM_ITERATIONS];

    printf("浮点运算测试 (%d 次迭代):\n", NUM_ITERATIONS);

    // 定点运算基准
    q_custom_t q_result = q_custom_from_float(1.0f);
    q_custom_t q_ops[NUM_ITERATIONS];

    printf("定点运算测试 (%d 次迭代):\n", NUM_ITERATIONS);

    printf("注意: 在真实环境中使用硬件计时器测量性能\n");
    printf("定点数运算通常比浮点运算更快，但精度较低\n");
}

#ifdef STB_fixed_TEST_MAIN
// 主测试函数
void run_all_tests(void) {
    printf("========================================\n");
    printf("    自定义定点数库测试套件\n");
    printf("    配置: Q%d.%d (总%d位)\n",
           CUSTOM_INT_BITS, CUSTOM_FRAC_BITS, CUSTOM_TOTAL_BITS);
    printf("========================================\n");

    test_configuration();
    test_conversion();
    test_precision();
    test_arithmetic();
    test_format_conversion();
    test_performance();

    printf("\n========================================\n");
    printf("            测试完成\n");
    printf("========================================\n");
}

#endif /* STB_fixed_TEST_MAIN */
#endif /* STB_fixed_TEST */

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// ==================== 配置选项 ====================
// 用户可配置的选项
#define ENABLE_Q8_8     1
#define ENABLE_Q16_16   1
#define ENABLE_Q32_32   1

// ==================== 定点数类型定义 ====================
typedef int16_t q8_8_t;     // Q8.8
typedef int32_t q16_16_t;   // Q16.16
typedef int64_t q32_32_t;   // Q32.32

// ==================== 通用宏定义 ====================
// 浮点数转定点数（四舍五入）
#define FLOAT_TO_Q(f, frac_bits, type) \
    ((type)((f) * (1ULL << (frac_bits)) + ((f) >= 0 ? 0.5f : -0.5f)))

// 浮点数转定点数（截断）
#define FLOAT_TO_Q_TRUNC(f, frac_bits, type) \
    ((type)((f) * (1ULL << (frac_bits))))

// 定点数转浮点数
#define Q_TO_FLOAT(q, frac_bits) \
    ((float)(q) / (float)(1ULL << (frac_bits)))

// 整数转定点数
#define INT_TO_Q(i, frac_bits, type) \
    ((type)(i) << (frac_bits))

// 定点数取整数部分
#define Q_TO_INT(q, frac_bits) \
    ((q) >> (frac_bits))

// 定点数取小数部分
#define Q_FRAC_PART(q, frac_bits) \
    ((q) & ((1ULL << (frac_bits)) - 1))

// 定点数四舍五入到整数
#define Q_ROUND_TO_INT(q, frac_bits) \
    (((q) + (1 << ((frac_bits) - 1))) >> (frac_bits))

// 定点数乘法（四舍五入）
#define Q_MUL(a, b, frac_bits, temp_type, result_type) \
    ((result_type)(((temp_type)(a) * (b) + (1LL << ((frac_bits) - 1))) >> (frac_bits)))

// 定点数除法（四舍五入）
#define Q_DIV(a, b, frac_bits, temp_type, result_type) \
    ((result_type)(((temp_type)(a) << (frac_bits)) / (b)))

// 定点数平方根（整数迭代法）
#define Q_SQRT(q, frac_bits, type) \
    q_sqrt_impl(q, frac_bits, (type)0)

// 定点数格式转换
#define Q_CONVERT(q, from_frac, to_frac, from_type, to_type) \
    ((from_frac) >= (to_frac) ? \
     ((to_type)((q) + (1LL << ((from_frac) - (to_frac) - 1))) >> ((from_frac) - (to_frac))) : \
     ((to_type)(q) << ((to_frac) - (from_frac))))

// ==================== Q8.8 定点数 ====================
#if ENABLE_Q8_8
    // Q8.8 常量
    #define Q8_8_FRAC_BITS    8
    #define Q8_8_INT_BITS     7
    #define Q8_8_TOTAL_BITS   16
    #define Q8_8_SCALE        (1 << Q8_8_FRAC_BITS)      // 256
    #define Q8_8_MAX          ((1LL << (Q8_8_TOTAL_BITS - 1)) - 1)  // 32767
    #define Q8_8_MIN          (-(1LL << (Q8_8_TOTAL_BITS - 1)))     // -32768
    #define Q8_8_MAX_FLOAT    ((float)Q8_8_MAX / Q8_8_SCALE)  // 127.99609375
    #define Q8_8_MIN_FLOAT    ((float)Q8_8_MIN / Q8_8_SCALE)  // -128.0
    #define Q8_8_PRECISION    (1.0f / Q8_8_SCALE)         // 0.00390625
    
    // Q8.8 掩码
    #define Q8_8_INT_MASK     0xFF00
    #define Q8_8_FRAC_MASK    0x00FF
    
    // Q8.8 转换函数
    static inline q8_8_t q8_8_from_float(float f) {
        return FLOAT_TO_Q(f, Q8_8_FRAC_BITS, q8_8_t);
    }
    
    static inline q8_8_t q8_8_from_float_trunc(float f) {
        return FLOAT_TO_Q_TRUNC(f, Q8_8_FRAC_BITS, q8_8_t);
    }
    
    static inline float q8_8_to_float(q8_8_t q) {
        return Q_TO_FLOAT(q, Q8_8_FRAC_BITS);
    }
    
    static inline q8_8_t q8_8_from_int(int16_t i) {
        return INT_TO_Q(i, Q8_8_FRAC_BITS, q8_8_t);
    }
    
    static inline int16_t q8_8_to_int(q8_8_t q) {
        return Q_TO_INT(q, Q8_8_FRAC_BITS);
    }
    
    static inline q8_8_t q8_8_frac_part(q8_8_t q) {
        return Q_FRAC_PART(q, Q8_8_FRAC_BITS);
    }
    
    static inline q8_8_t q8_8_round_to_int(q8_8_t q) {
        return Q_ROUND_TO_INT(q, Q8_8_FRAC_BITS);
    }
    
    // Q8.8 算术运算
    static inline q8_8_t q8_8_add(q8_8_t a, q8_8_t b) { return a + b; }
    static inline q8_8_t q8_8_sub(q8_8_t a, q8_8_t b) { return a - b; }
    static inline q8_8_t q8_8_neg(q8_8_t a) { return -a; }
    
    static inline q8_8_t q8_8_mul(q8_8_t a, q8_8_t b) {
        return Q_MUL(a, b, Q8_8_FRAC_BITS, int32_t, q8_8_t);
    }
    
    static inline q8_8_t q8_8_div(q8_8_t a, q8_8_t b) {
        return Q_DIV(a, b, Q8_8_FRAC_BITS, int32_t, q8_8_t);
    }
    
    // Q8.8 平方根（整数迭代法）
    static inline q8_8_t q8_8_sqrt(q8_8_t q) {
        if (q <= 0) return 0;
        
        uint32_t x = q << Q8_8_FRAC_BITS;  // 转换为 Q16.16 格式进行计算
        uint32_t res = 0;
        uint32_t bit = 1U << 30;  // 从最高位开始
        
        while (bit > x) {
            bit >>= 2;
        }
        
        while (bit != 0) {
            if (x >= res + bit) {
                x -= res + bit;
                res = (res >> 1) + bit;
            } else {
                res >>= 1;
            }
            bit >>= 2;
        }
        
        return (q8_8_t)(res >> (Q8_8_FRAC_BITS / 2));
    }
    
    // Q8.8 绝对值
    static inline q8_8_t q8_8_abs(q8_8_t q) {
        return q < 0 ? -q : q;
    }
    
    // Q8.8 比较
    static inline bool q8_8_eq(q8_8_t a, q8_8_t b) { return a == b; }
    static inline bool q8_8_lt(q8_8_t a, q8_8_t b) { return a < b; }
    static inline bool q8_8_gt(q8_8_t a, q8_8_t b) { return a > b; }
    static inline bool q8_8_le(q8_8_t a, q8_8_t b) { return a <= b; }
    static inline bool q8_8_ge(q8_8_t a, q8_8_t b) { return a >= b; }
#endif

// ==================== Q16.16 定点数 ====================
#if ENABLE_Q16_16
    // Q16.16 常量
    #define Q16_16_FRAC_BITS    16
    #define Q16_16_INT_BITS     15
    #define Q16_16_TOTAL_BITS   32
    #define Q16_16_SCALE        (1ULL << Q16_16_FRAC_BITS)      // 65536
    #define Q16_16_MAX          ((1LL << (Q16_16_TOTAL_BITS - 1)) - 1)  // 2147483647
    #define Q16_16_MIN          (-(1LL << (Q16_16_TOTAL_BITS - 1)))     // -2147483648
    #define Q16_16_MAX_FLOAT    ((float)Q16_16_MAX / Q16_16_SCALE)  // 32767.9999847
    #define Q16_16_MIN_FLOAT    ((float)Q16_16_MIN / Q16_16_SCALE)  // -32768.0
    #define Q16_16_PRECISION    (1.0f / Q16_16_SCALE)         // 0.000015258789
    
    // Q16.16 掩码
    #define Q16_16_INT_MASK     0xFFFF0000
    #define Q16_16_FRAC_MASK    0x0000FFFF
    
    // Q16.16 转换函数
    static inline q16_16_t q16_16_from_float(float f) {
        return FLOAT_TO_Q(f, Q16_16_FRAC_BITS, q16_16_t);
    }
    
    static inline q16_16_t q16_16_from_float_trunc(float f) {
        return FLOAT_TO_Q_TRUNC(f, Q16_16_FRAC_BITS, q16_16_t);
    }
    
    static inline float q16_16_to_float(q16_16_t q) {
        return Q_TO_FLOAT(q, Q16_16_FRAC_BITS);
    }
    
    static inline q16_16_t q16_16_from_int(int32_t i) {
        return INT_TO_Q(i, Q16_16_FRAC_BITS, q16_16_t);
    }
    
    static inline int32_t q16_16_to_int(q16_16_t q) {
        return Q_TO_INT(q, Q16_16_FRAC_BITS);
    }
    
    static inline q16_16_t q16_16_frac_part(q16_16_t q) {
        return Q_FRAC_PART(q, Q16_16_FRAC_BITS);
    }
    
    static inline q16_16_t q16_16_round_to_int(q16_16_t q) {
        return Q_ROUND_TO_INT(q, Q16_16_FRAC_BITS);
    }
    
    // Q16.16 算术运算
    static inline q16_16_t q16_16_add(q16_16_t a, q16_16_t b) { return a + b; }
    static inline q16_16_t q16_16_sub(q16_16_t a, q16_16_t b) { return a - b; }
    static inline q16_16_t q16_16_neg(q16_16_t a) { return -a; }
    
    static inline q16_16_t q16_16_mul(q16_16_t a, q16_16_t b) {
        return Q_MUL(a, b, Q16_16_FRAC_BITS, int64_t, q16_16_t);
    }
    
    static inline q16_16_t q16_16_div(q16_16_t a, q16_16_t b) {
        return Q_DIV(a, b, Q16_16_FRAC_BITS, int64_t, q16_16_t);
    }
    
    // Q16.16 平方根（整数迭代法）
    static inline q16_16_t q16_16_sqrt(q16_16_t q) {
        if (q <= 0) return 0;
        
        uint64_t x = (uint64_t)q << Q16_16_FRAC_BITS;  // 转换为 Q32.32 格式进行计算
        uint64_t res = 0;
        uint64_t bit = 1ULL << 62;  // 从最高位开始
        
        while (bit > x) {
            bit >>= 2;
        }
        
        while (bit != 0) {
            if (x >= res + bit) {
                x -= res + bit;
                res = (res >> 1) + bit;
            } else {
                res >>= 1;
            }
            bit >>= 2;
        }
        
        return (q16_16_t)(res >> (Q16_16_FRAC_BITS / 2));
    }
    
    // Q16.16 绝对值
    static inline q16_16_t q16_16_abs(q16_16_t q) {
        return q < 0 ? -q : q;
    }
    
    // Q16.16 比较
    static inline bool q16_16_eq(q16_16_t a, q16_16_t b) { return a == b; }
    static inline bool q16_16_lt(q16_16_t a, q16_16_t b) { return a < b; }
    static inline bool q16_16_gt(q16_16_t a, q16_16_t b) { return a > b; }
    static inline bool q16_16_le(q16_16_t a, q16_16_t b) { return a <= b; }
    static inline bool q16_16_ge(q16_16_t a, q16_16_t b) { return a >= b; }
#endif

// ==================== Q32.32 定点数 ====================
#if ENABLE_Q32_32
    // Q32.32 常量
    #define Q32_32_FRAC_BITS    32
    #define Q32_32_INT_BITS     31
    #define Q32_32_TOTAL_BITS   64
    #define Q32_32_SCALE        (1ULL << Q32_32_FRAC_BITS)      // 4294967296
    #define Q32_32_MAX          ((1LL << (Q32_32_TOTAL_BITS - 1)) - 1)  // 9223372036854775807
    #define Q32_32_MIN          (-(1LL << (Q32_32_TOTAL_BITS - 1)))     // -9223372036854775808
    #define Q32_32_MAX_FLOAT    ((float)Q32_32_MAX / Q32_32_SCALE)  // 2147483647.9999998
    #define Q32_32_MIN_FLOAT    ((float)Q32_32_MIN / Q32_32_SCALE)  // -2147483648.0
    #define Q32_32_PRECISION    (1.0f / Q32_32_SCALE)         // 2.3283064e-10
    
    // Q32.32 掩码
    #define Q32_32_INT_MASK     0xFFFFFFFF00000000ULL
    #define Q32_32_FRAC_MASK    0x00000000FFFFFFFFULL
    
    // Q32.32 转换函数
    static inline q32_32_t q32_32_from_float(float f) {
        return FLOAT_TO_Q(f, Q32_32_FRAC_BITS, q32_32_t);
    }
    
    static inline q32_32_t q32_32_from_float_trunc(float f) {
        return FLOAT_TO_Q_TRUNC(f, Q32_32_FRAC_BITS, q32_32_t);
    }
    
    static inline q32_32_t q32_32_from_double(double d) {
        return (q32_32_t)(d * (1ULL << Q32_32_FRAC_BITS) + (d >= 0 ? 0.5 : -0.5));
    }
    
    static inline float q32_32_to_float(q32_32_t q) {
        return Q_TO_FLOAT(q, Q32_32_FRAC_BITS);
    }
    
    static inline double q32_32_to_double(q32_32_t q) {
        return (double)q / (double)(1ULL << Q32_32_FRAC_BITS);
    }
    
    static inline q32_32_t q32_32_from_int(int64_t i) {
        return INT_TO_Q(i, Q32_32_FRAC_BITS, q32_32_t);
    }
    
    static inline int64_t q32_32_to_int(q32_32_t q) {
        return Q_TO_INT(q, Q32_32_FRAC_BITS);
    }
    
    static inline q32_32_t q32_32_frac_part(q32_32_t q) {
        return Q_FRAC_PART(q, Q32_32_FRAC_BITS);
    }
    
    static inline q32_32_t q32_32_round_to_int(q32_32_t q) {
        return Q_ROUND_TO_INT(q, Q32_32_FRAC_BITS);
    }
    
    // Q32.32 算术运算
    static inline q32_32_t q32_32_add(q32_32_t a, q32_32_t b) { return a + b; }
    static inline q32_32_t q32_32_sub(q32_32_t a, q32_32_t b) { return a - b; }
    static inline q32_32_t q32_32_neg(q32_32_t a) { return -a; }
    
    // Q32.32 乘法（使用128位中间结果）
    static inline q32_32_t q32_32_mul(q32_32_t a, q32_32_t b) {
        __int128 temp = (__int128)a * b;
        return (q32_32_t)((temp + (1LL << (Q32_32_FRAC_BITS - 1))) >> Q32_32_FRAC_BITS);
    }
    
    // Q32.32 除法（使用128位中间结果）
    static inline q32_32_t q32_32_div(q32_32_t a, q32_32_t b) {
        __int128 temp = (__int128)a << Q32_32_FRAC_BITS;
        return (q32_32_t)(temp / b);
    }
    
    // Q32.32 平方根（整数迭代法）
    static inline q32_32_t q32_32_sqrt(q32_32_t q) {
        if (q <= 0) return 0;
        
        unsigned __int128 x = (unsigned __int128)q << Q32_32_FRAC_BITS;
        unsigned __int128 res = 0;
        unsigned __int128 bit = (unsigned __int128)1 << 126;  // 从最高位开始
        
        while (bit > x) {
            bit >>= 2;
        }
        
        while (bit != 0) {
            if (x >= res + bit) {
                x -= res + bit;
                res = (res >> 1) + bit;
            } else {
                res >>= 1;
            }
            bit >>= 2;
        }
        
        return (q32_32_t)(res >> (Q32_32_FRAC_BITS / 2));
    }
    
    // Q32.32 绝对值
    static inline q32_32_t q32_32_abs(q32_32_t q) {
        return q < 0 ? -q : q;
    }
    
    // Q32.32 比较
    static inline bool q32_32_eq(q32_32_t a, q32_32_t b) { return a == b; }
    static inline bool q32_32_lt(q32_32_t a, q32_32_t b) { return a < b; }
    static inline bool q32_32_gt(q32_32_t a, q32_32_t b) { return a > b; }
    static inline bool q32_32_le(q32_32_t a, q32_32_t b) { return a <= b; }
    static inline bool q32_32_ge(q32_32_t a, q32_32_t b) { return a >= b; }
#endif

// ==================== 格式转换函数 ====================
// Q8.8 转 Q16.16
static inline q16_16_t q8_8_to_q16_16(q8_8_t q) {
    return (q16_16_t)q << (Q16_16_FRAC_BITS - Q8_8_FRAC_BITS);
}

// Q8.8 转 Q32.32
static inline q32_32_t q8_8_to_q32_32(q8_8_t q) {
    return (q32_32_t)q << (Q32_32_FRAC_BITS - Q8_8_FRAC_BITS);
}

// Q16.16 转 Q8.8（四舍五入）
static inline q8_8_t q16_16_to_q8_8(q16_16_t q) {
    return (q8_8_t)((q + (1 << (Q16_16_FRAC_BITS - Q8_8_FRAC_BITS - 1))) >> 
                    (Q16_16_FRAC_BITS - Q8_8_FRAC_BITS));
}

// Q16.16 转 Q32.32
static inline q32_32_t q16_16_to_q32_32(q16_16_t q) {
    return (q32_32_t)q << (Q32_32_FRAC_BITS - Q16_16_FRAC_BITS);
}

// Q32.32 转 Q8.8（四舍五入）
static inline q8_8_t q32_32_to_q8_8(q32_32_t q) {
    return (q8_8_t)((q + (1LL << (Q32_32_FRAC_BITS - Q8_8_FRAC_BITS - 1))) >> 
                    (Q32_32_FRAC_BITS - Q8_8_FRAC_BITS));
}

// Q32.32 转 Q16.16（四舍五入）
static inline q16_16_t q32_32_to_q16_16(q32_32_t q) {
    return (q16_16_t)((q + (1LL << (Q32_32_FRAC_BITS - Q16_16_FRAC_BITS - 1))) >> 
                      (Q32_32_FRAC_BITS - Q16_16_FRAC_BITS));
}


#ifdef STB_fixed_8_16_32_TEST
// ==================== 测试函数 ====================
void print_config_info() {
    printf("定点数配置信息\n");
    printf("==============\n");
    
#if ENABLE_Q8_8
    printf("\nQ8.8 配置:\n");
    printf("  整数位数: %d 位\n", Q8_8_INT_BITS);
    printf("  小数位数: %d 位\n", Q8_8_FRAC_BITS);
    printf("  总位数:   %d 位\n", Q8_8_TOTAL_BITS);
    printf("  缩放因子: %u\n", Q8_8_SCALE);
    printf("  最大值:   %d (0x%04X)\n", Q8_8_MAX, Q8_8_MAX);
    printf("  最小值:   %d (0x%04X)\n", Q8_8_MIN, Q8_8_MIN);
    printf("  浮点范围: [%f, %f]\n", Q8_8_MIN_FLOAT, Q8_8_MAX_FLOAT);
    printf("  精度:     %.10f\n", Q8_8_PRECISION);
#endif

#if ENABLE_Q16_16
    printf("\nQ16.16 配置:\n");
    printf("  整数位数: %d 位\n", Q16_16_INT_BITS);
    printf("  小数位数: %d 位\n", Q16_16_FRAC_BITS);
    printf("  总位数:   %d 位\n", Q16_16_TOTAL_BITS);
    printf("  缩放因子: %llu\n", (unsigned long long)Q16_16_SCALE);
    printf("  最大值:   %d (0x%08X)\n", Q16_16_MAX, Q16_16_MAX);
    printf("  最小值:   %d (0x%08X)\n", Q16_16_MIN, Q16_16_MIN);
    printf("  浮点范围: [%f, %f]\n", Q16_16_MIN_FLOAT, Q16_16_MAX_FLOAT);
    printf("  精度:     %.15f\n", Q16_16_PRECISION);
#endif

#if ENABLE_Q32_32
    printf("\nQ32.32 配置:\n");
    printf("  整数位数: %d 位\n", Q32_32_INT_BITS);
    printf("  小数位数: %d 位\n", Q32_32_FRAC_BITS);
    printf("  总位数:   %d 位\n", Q32_32_TOTAL_BITS);
    printf("  缩放因子: %llu\n", (unsigned long long)Q32_32_SCALE);
    printf("  最大值:   %lld (0x%016llX)\n", (long long)Q32_32_MAX, (unsigned long long)Q32_32_MAX);
    printf("  最小值:   %lld (0x%016llX)\n", (long long)Q32_32_MIN, (unsigned long long)Q32_32_MIN);
    printf("  浮点范围: [%f, %f]\n", Q32_32_MIN_FLOAT, Q32_32_MAX_FLOAT);
    printf("  精度:     %.20f\n", Q32_32_PRECISION);
#endif
    
    printf("\n");
}

void test_q8_8_basic() {
#if ENABLE_Q8_8
    printf("=== 测试 Q8.8 基本功能 ===\n");
    
    float test_values[] = {3.14159f, -2.718f, 127.5f, -128.0f, 0.0f, 0.5f, -0.5f, 1.0f, -1.0f};
    
    for (int i = 0; i < sizeof(test_values)/sizeof(test_values[0]); i++) {
        q8_8_t q_round = q8_8_from_float(test_values[i]);
        q8_8_t q_trunc = q8_8_from_float_trunc(test_values[i]);
        float back_round = q8_8_to_float(q_round);
        float back_trunc = q8_8_to_float(q_trunc);
        
        printf("浮点数: %9.5f\n", test_values[i]);
        printf("  四舍五入: 0x%04X = %9.5f (误差: %9.5f)\n", 
               (uint16_t)q_round, back_round, fabsf(test_values[i] - back_round));
        printf("  截断:     0x%04X = %9.5f (误差: %9.5f)\n", 
               (uint16_t)q_trunc, back_trunc, fabsf(test_values[i] - back_trunc));
        
        // 测试整数和小数部分
        int16_t int_part = q8_8_to_int(q_round);
        q8_8_t frac_part = q8_8_frac_part(q_round);
        printf("  整数部分: %d, 小数部分: 0x%02X = %9.5f\n", 
               int_part, (uint8_t)frac_part, q8_8_to_float(frac_part));
    }
    
    printf("\n");
#endif
}

void test_q16_16_basic() {
#if ENABLE_Q16_16
    printf("=== 测试 Q16.16 基本功能 ===\n");
    
    float test_values[] = {3.14159265f, -2.7182818f, 32767.999f, -32768.0f, 0.0f, 0.5f, -0.5f, 1000.123f, -1000.123f};
    
    for (int i = 0; i < sizeof(test_values)/sizeof(test_values[0]); i++) {
        q16_16_t q_round = q16_16_from_float(test_values[i]);
        q16_16_t q_trunc = q16_16_from_float_trunc(test_values[i]);
        float back_round = q16_16_to_float(q_round);
        float back_trunc = q16_16_to_float(q_trunc);
        
        printf("浮点数: %13.8f\n", test_values[i]);
        printf("  四舍五入: 0x%08X = %13.8f (误差: %13.8f)\n", 
               (uint32_t)q_round, back_round, fabsf(test_values[i] - back_round));
        printf("  截断:     0x%08X = %13.8f (误差: %13.8f)\n", 
               (uint32_t)q_trunc, back_trunc, fabsf(test_values[i] - back_trunc));
        
        // 测试整数和小数部分
        int32_t int_part = q16_16_to_int(q_round);
        q16_16_t frac_part = q16_16_frac_part(q_round);
        printf("  整数部分: %d, 小数部分: 0x%04X = %13.8f\n", 
               int_part, (uint16_t)frac_part, q16_16_to_float(frac_part));
    }
    
    printf("\n");
#endif
}

void test_q32_32_basic() {
#if ENABLE_Q32_32
    printf("=== 测试 Q32.32 基本功能 ===\n");
    
    double test_values[] = {3.141592653589793, -2.718281828459045, 2147483647.999999, 
                           -2147483648.0, 0.0, 0.5, -0.5, 1000000.123456, -1000000.123456};
    
    for (int i = 0; i < sizeof(test_values)/sizeof(test_values[0]); i++) {
        q32_32_t q_round = q32_32_from_double(test_values[i]);
        double back_round = q32_32_to_double(q_round);
        
        printf("浮点数: %20.15f\n", test_values[i]);
        printf("  Q32.32: 0x%016llX = %20.15f\n", 
               (unsigned long long)q_round, back_round);
        printf("  误差: %20.15f\n", fabs(test_values[i] - back_round));
        
        // 测试整数和小数部分
        int64_t int_part = q32_32_to_int(q_round);
        q32_32_t frac_part = q32_32_frac_part(q_round);
        printf("  整数部分: %lld, 小数部分: 0x%08llX\n", 
               (long long)int_part, (unsigned long long)frac_part);
    }
    
    printf("\n");
#endif
}

void test_arithmetic_operations() {
    printf("=== 测试算术运算 ===\n");
    
#if ENABLE_Q8_8
    printf("\nQ8.8 算术运算:\n");
    q8_8_t a_q8 = q8_8_from_float(3.5f);
    q8_8_t b_q8 = q8_8_from_float(2.0f);
    
    printf("  A = 3.5, B = 2.0\n");
    printf("  A + B = %9.5f\n", q8_8_to_float(q8_8_add(a_q8, b_q8)));
    printf("  A - B = %9.5f\n", q8_8_to_float(q8_8_sub(a_q8, b_q8)));
    printf("  A * B = %9.5f\n", q8_8_to_float(q8_8_mul(a_q8, b_q8)));
    printf("  A / B = %9.5f\n", q8_8_to_float(q8_8_div(a_q8, b_q8)));
    printf("  -A    = %9.5f\n", q8_8_to_float(q8_8_neg(a_q8)));
    printf("  |A|   = %9.5f\n", q8_8_to_float(q8_8_abs(a_q8)));
    printf("  sqrt(A) = %9.5f\n", q8_8_to_float(q8_8_sqrt(a_q8)));
#endif

#if ENABLE_Q16_16
    printf("\nQ16.16 算术运算:\n");
    q16_16_t a_q16 = q16_16_from_float(3.14159265f);
    q16_16_t b_q16 = q16_16_from_float(2.0f);
    
    printf("  A = π, B = 2.0\n");
    printf("  A + B = %13.8f\n", q16_16_to_float(q16_16_add(a_q16, b_q16)));
    printf("  A - B = %13.8f\n", q16_16_to_float(q16_16_sub(a_q16, b_q16)));
    printf("  A * B = %13.8f\n", q16_16_to_float(q16_16_mul(a_q16, b_q16)));
    printf("  A / B = %13.8f\n", q16_16_to_float(q16_16_div(a_q16, b_q16)));
    printf("  -A    = %13.8f\n", q16_16_to_float(q16_16_neg(a_q16)));
    printf("  |A|   = %13.8f\n", q16_16_to_float(q16_16_abs(a_q16)));
    printf("  sqrt(A) = %13.8f\n", q16_16_to_float(q16_16_sqrt(q16_16_from_float(2.0f))));
#endif

#if ENABLE_Q32_32
    printf("\nQ32.32 算术运算:\n");
    q32_32_t a_q32 = q32_32_from_double(3.141592653589793);
    q32_32_t b_q32 = q32_32_from_double(2.0);
    
    printf("  A = π, B = 2.0\n");
    printf("  A + B = %20.15f\n", q32_32_to_double(q32_32_add(a_q32, b_q32)));
    printf("  A - B = %20.15f\n", q32_32_to_double(q32_32_sub(a_q32, b_q32)));
    printf("  A * B = %20.15f\n", q32_32_to_double(q32_32_mul(a_q32, b_q32)));
    printf("  A / B = %20.15f\n", q32_32_to_double(q32_32_div(a_q32, b_q32)));
    printf("  -A    = %20.15f\n", q32_32_to_double(q32_32_neg(a_q32)));
    printf("  |A|   = %20.15f\n", q32_32_to_double(q32_32_abs(a_q32)));
    printf("  sqrt(A) = %20.15f\n", q32_32_to_double(q32_32_sqrt(q32_32_from_double(2.0))));
#endif
}

void test_conversion_operations() {
    printf("\n=== 测试格式转换 ===\n");
    
#if ENABLE_Q8_8 && ENABLE_Q16_16 && ENABLE_Q32_32
    float test_value = 3.14159f;
    
    printf("测试值: %f\n\n", test_value);
    
    // 转换到不同格式
    q8_8_t q8 = q8_8_from_float(test_value);
    q16_16_t q16_from_q8 = q8_8_to_q16_16(q8);
    q32_32_t q32_from_q8 = q8_8_to_q32_32(q8);
    
    q16_16_t q16 = q16_16_from_float(test_value);
    q8_8_t q8_from_q16 = q16_16_to_q8_8(q16);
    q32_32_t q32_from_q16 = q16_16_to_q32_32(q16);
    
    q32_32_t q32 = q32_32_from_double(test_value);
    q8_8_t q8_from_q32 = q32_32_to_q8_8(q32);
    q16_16_t q16_from_q32 = q32_32_to_q16_16(q32);
    
    printf("Q8.8 -> Q16.16:\n");
    printf("  Q8.8:  0x%04X = %9.5f\n", (uint16_t)q8, q8_8_to_float(q8));
    printf("  Q16.16:0x%08X = %9.5f\n", (uint32_t)q16_from_q8, q16_16_to_float(q16_from_q8));
    printf("  误差: %9.5f\n\n", fabsf(test_value - q16_16_to_float(q16_from_q8)));
    
    printf("Q8.8 -> Q32.32:\n");
    printf("  Q8.8:  0x%04X = %9.5f\n", (uint16_t)q8, q8_8_to_float(q8));
    printf("  Q32.32:0x%016llX = %9.5f\n", (unsigned long long)q32_from_q8, q32_32_to_float(q32_from_q8));
    printf("  误差: %9.5f\n\n", fabsf(test_value - q32_32_to_float(q32_from_q8)));
    
    printf("Q16.16 -> Q8.8:\n");
    printf("  Q16.16:0x%08X = %13.8f\n", (uint32_t)q16, q16_16_to_float(q16));
    printf("  Q8.8:  0x%04X = %9.5f\n", (uint16_t)q8_from_q16, q8_8_to_float(q8_from_q16));
    printf("  精度损失: %9.5f\n\n", fabsf(q16_16_to_float(q16) - q8_8_to_float(q8_from_q16)));
    
    printf("Q32.32 -> Q8.8:\n");
    printf("  Q32.32:0x%016llX = %20.15f\n", (unsigned long long)q32, q32_32_to_double(q32));
    printf("  Q8.8:  0x%04X = %9.5f\n", (uint16_t)q8_from_q32, q8_8_to_float(q8_from_q32));
    printf("  精度损失: %9.5f\n", fabsf(q32_32_to_double(q32) - q8_8_to_float(q8_from_q32)));
#endif
}

void test_edge_cases() {
    printf("\n=== 测试边界情况 ===\n");
    
#if ENABLE_Q8_8
    printf("\nQ8.8 边界情况:\n");
    printf("  最大值测试:\n");
    printf("    浮点数: %f\n", Q8_8_MAX_FLOAT);
    printf("    定点数: 0x%04X\n", Q8_8_MAX);
    printf("    转换回: %f\n", q8_8_to_float(Q8_8_MAX));
    
    printf("\n  最小值测试:\n");
    printf("    浮点数: %f\n", Q8_8_MIN_FLOAT);
    printf("    定点数: 0x%04X\n", (uint16_t)Q8_8_MIN);
    printf("    转换回: %f\n", q8_8_to_float(Q8_8_MIN));
    
    printf("\n  零测试:\n");
    q8_8_t zero = q8_8_from_float(0.0f);
    printf("    零值: 0x%04X\n", zero);
    printf("    绝对值: 0x%04X\n", q8_8_abs(zero));
    printf("    取反: 0x%04X\n", q8_8_neg(zero));
#endif

#if ENABLE_Q16_16
    printf("\nQ16.16 边界情况:\n");
    printf("  最大值测试:\n");
    printf("    浮点数: %f\n", Q16_16_MAX_FLOAT);
    printf("    定点数: 0x%08X\n", Q16_16_MAX);
    printf("    转换回: %f\n", q16_16_to_float(Q16_16_MAX));
    
    printf("\n  最小值测试:\n");
    printf("    浮点数: %f\n", Q16_16_MIN_FLOAT);
    printf("    定点数: 0x%08X\n", (uint32_t)Q16_16_MIN);
    printf("    转换回: %f\n", q16_16_to_float(Q16_16_MIN));
#endif

#if ENABLE_Q32_32
    printf("\nQ32.32 边界情况:\n");
    printf("  最大值测试:\n");
    printf("    浮点数: %f\n", Q32_32_MAX_FLOAT);
    printf("    定点数: 0x%016llX\n", (unsigned long long)Q32_32_MAX);
    printf("    转换回: %f\n", q32_32_to_float(Q32_32_MAX));
    
    printf("\n  最小值测试:\n");
    printf("    浮点数: %f\n", Q32_32_MIN_FLOAT);
    printf("    定点数: 0x%016llX\n", (unsigned long long)Q32_32_MIN);
    printf("    转换回: %f\n", q32_32_to_float(Q32_32_MIN));
#endif
}

void test_performance() {
    printf("\n=== 性能测试 ===\n");
    
#if ENABLE_Q8_8
    printf("\nQ8.8 性能测试 (1000000次运算):\n");
    int iterations = 1000000;
    q8_8_t sum_q8 = q8_8_from_float(0.0f);
    q8_8_t a_q8 = q8_8_from_float(1.5f);
    q8_8_t b_q8 = q8_8_from_float(2.5f);
    
    clock_t start = clock();
    for (int i = 0; i < iterations; i++) {
        sum_q8 = q8_8_add(sum_q8, q8_8_mul(a_q8, b_q8));
    }
    clock_t end = clock();
    printf("  乘法加法耗时: %.3f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
    printf("  结果: %f\n", q8_8_to_float(sum_q8));
#endif

#if ENABLE_Q16_16
    printf("\nQ16.16 性能测试 (1000000次运算):\n");
    q16_16_t sum_q16 = q16_16_from_float(0.0f);
    q16_16_t a_q16 = q16_16_from_float(1.5f);
    q16_16_t b_q16 = q16_16_from_float(2.5f);
    
    start = clock();
    for (int i = 0; i < iterations; i++) {
        sum_q16 = q16_16_add(sum_q16, q16_16_mul(a_q16, b_q16));
    }
    end = clock();
    printf("  乘法加法耗时: %.3f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
    printf("  结果: %f\n", q16_16_to_float(sum_q16));
#endif
}

#ifdef STB_fixed_8_16_32_TEST_MAIN
int main() {
    printf("定点数系统测试 (Q8.8, Q16.16, Q32.32)\n");
    printf("===================================\n\n");
    
    // 打印配置信息
    print_config_info();
    
    // 运行测试
    test_q8_8_basic();
    test_q16_16_basic();
    test_q32_32_basic();
    test_arithmetic_operations();
    test_conversion_operations();
    test_edge_cases();
    test_performance();
    
    printf("\n所有测试完成。\n");
    return 0;
}
#endif /* STB_fixed_8_16_32_TEST_MAIN */
#endif /* STB_fixed_8_16_32_TEST */
/**The configuration item
 * 配置项
 * 
 * */

/**The Usage
 * 使用方法
 * #include "nop.h"
 * */

/* nop.h - 通用 NOP 宏/函数
 * nop.h - Universal NOP macro/function
 *
 * 兼容 C99 标准，适配主流编译器。
 * Compatible with C99 standard, adapted to mainstream compilers.
 * 使用 static inline 函数和宏定义，确保在所有模式（Debug/Release）下均有效。
 * Uses static inline functions and macros to ensure effectiveness in all modes (Debug/Release).
 */

#ifndef _NOP_H_
#define _NOP_H_

/* 
 * MSVC 编译器
 * MSVC compiler
 * 需要包含 <intrin.h> 才能使用 __nop()
 * Need to include <intrin.h> to use __nop()
 */
#if defined(_MSC_VER)
    #include <intrin.h>
    #pragma intrinsic(__nop)  // 确保内联  // Ensure inlining
    static inline void cpu_nop(void) {
        __nop();  // MSVC 内置 NOP  // MSVC built-in NOP
    }

/* 
 * GCC/Clang 编译器 (以及兼容的编译器如 MinGW)
 * GCC/Clang compilers (and compatible compilers like MinGW)
 * 使用内联汇编实现 NOP。
 * Use inline assembly to implement NOP.
 * - __builtin_nop() 是 GCC 12+ 的内置函数，兼容性最好。
 * - __builtin_nop() is a built-in function of GCC 12+ with best compatibility.
 * - __asm__ __volatile__("nop") 是通用写法。
 * - __asm__ __volatile__("nop") is a common approach.
 */
#elif defined(__GNUC__) || defined(__clang__)  // GCC/Clang
    static inline void cpu_nop(void) {
        #if __has_builtin(__builtin_nop)  // GCC 12+ 或支持的 Clang  // GCC 12+ or supported Clang
            __builtin_nop();  // 直接调用内置函数，最安全  // Direct built-in call, safest
        #else
            __asm__ __volatile__("nop");  // x86/x64 常用 NOP  // Common NOP for x86/x64
        #endif
    }

/* 
 * Intel ICC 编译器
 * Intel ICC compiler
 * ICC 兼容 GCC 的内联汇编语法。
 * ICC is compatible with GCC inline assembly syntax.
 */
#elif defined(__INTEL_COMPILER)
    static inline void cpu_nop(void) {
        __asm__ __volatile__("nop");
    }

/* 
 * ARMCC 编译器 (Keil MDK/ARMCC)
 * ARMCC compiler (Keil MDK/ARMCC)
 * 使用 ARM 的专用指令 __NOP()。
 * Use ARM specific instruction __NOP().
 */
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
    #include <arm_acle.h>  // 包含 ARM 内置函数定义  // Include ARM built-in definitions
    static inline void cpu_nop(void) {
        __NOP();  // ARM 编译器内置 NOP  // ARM compiler built-in NOP
    }

/* 
 * 通用回退方案
 * General fallback solution
 * 当编译器不支持任何内联汇编或内置函数时，使用 volatile 空语句。
 * When compiler doesn't support inline assembly or built-ins, use volatile empty statement.
 * 注意：这种方式 **不一定生成机器码 NOP**，可能被优化掉。
 * Note: This method **may not generate machine code NOP** and could be optimized away.
 */
#else
    static inline void cpu_nop(void) {
        /* 
         * 通过读取一个 volatile 变量来制造“读操作”，
         * By reading a volatile variable to create a "read operation",
         * 通常会生成类似 NOP 的机器码（如 mov eax, eax）。
         * typically generates NOP-like machine code (e.g., mov eax, eax).
         */
        volatile int dummy = 0;
        (void)dummy;
    }
#endif

/* 
 * 统一的宏定义
 * Unified macro definition
 * 如果你更喜欢宏形式，可以使用 CPU_NOP()。
 * If you prefer macro form, use CPU_NOP().
 * 该宏直接映射到 cpu_nop() 函数。
 * This macro directly maps to cpu_nop() function.
 */
#define CPU_NOP()  cpu_nop()

#endif /* _NOP_H_ */

/**The configuration item
 * 
 * */

/**The Usage
 * #include "nop.h"
 * */

/* nop.h - 通用 NOP 宏/函数
 *
 * 兼容 C99 标准，适配主流编译器。
 * 使用 static inline 函数和宏定义，确保在所有模式（Debug/Release）下均有效。
 */

#ifndef _NOP_H_
#define _NOP_H_

/* 
 * MSVC 编译器
 * 需要包含 <intrin.h> 才能使用 __nop()
 */
#if defined(_MSC_VER)
    #include <intrin.h>
    #pragma intrinsic(__nop)  // 确保内联
    static inline void cpu_nop(void) {
        __nop();  // MSVC 内置 NOP
    }

/* 
 * GCC/Clang 编译器 (以及兼容的编译器如 MinGW)
 * 使用内联汇编实现 NOP。
 * - __builtin_nop() 是 GCC 12+ 的内置函数，兼容性最好。
 * - __asm__ __volatile__("nop") 是通用写法。
 */
#elif defined(__GNUC__) || defined(__clang__)  // GCC/Clang
    static inline void cpu_nop(void) {
        #if __has_builtin(__builtin_nop)  // GCC 12+ 或支持的 Clang
            __builtin_nop();  // 直接调用内置函数，最安全
        #else
            __asm__ __volatile__("nop");  // x86/x64 常用 NOP
        #endif
    }

/* 
 * Intel ICC 编译器
 * ICC 兼容 GCC 的内联汇编语法。
 */
#elif defined(__INTEL_COMPILER)
    static inline void cpu_nop(void) {
        __asm__ __volatile__("nop");
    }

/* 
 * ARMCC 编译器 (Keil MDK/ARMCC)
 * 使用 ARM 的专用指令 __NOP()。
 */
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
    #include <arm_acle.h>  // 包含 ARM 内置函数定义
    static inline void cpu_nop(void) {
        __NOP();  // ARM 编译器内置 NOP
    }

/* 
 * 通用回退方案
 * 当编译器不支持任何内联汇编或内置函数时，使用 volatile 空语句。
 * 注意：这种方式 **不一定生成机器码 NOP**，可能被优化掉。
 */
#else
    static inline void cpu_nop(void) {
        /* 
         * 通过读取一个 volatile 变量来制造“读操作”，
         * 通常会生成类似 NOP 的机器码（如 mov eax, eax）。
         */
        volatile int dummy = 0;
        (void)dummy;
    }
#endif

/* 
 * 统一的宏定义
 * 如果你更喜欢宏形式，可以使用 CPU_NOP()。
 * 该宏直接映射到 cpu_nop() 函数。
 */
#define CPU_NOP()  cpu_nop()

#endif /* _NOP_H_ */

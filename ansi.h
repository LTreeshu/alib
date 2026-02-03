/**deque.h configuration item
 * #define USE_Unicode
 * */

/**The Usage
 * #include "ansi.h"
 * */

#ifndef _ANSI_H_
#define _ANSI_H_ 

/* ANSI颜色代码 */
#define ANSI_RESET   "\033[0m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_RED     "\033[31m"
#define STB_ANSI_BOLD    "\033[1m"

/* 二值结果打印 */
/* Unicode符号（跨平台兼容） */
#define SYMBOL_TICK  "✓"  /* ✓ UTF-8 */
#define SYMBOL_CROSS "✗"  /* ✗ UTF-8 */

#ifdef USE_Unicode

#define COLOR_y ANSI_GREEN SYMBOL_TICK  ANSI_RESET
#define COLOR_n ANSI_RED   SYMBOL_CROSS ANSI_RESET

#else

#define COLOR_y ANSI_GREEN "[V]" ANSI_RESET
#define COLOR_n ANSI_RED   "[X]" ANSI_RESET

#endif

#define YorN(y)   (y)? SYMBOL_TICK : SYMBOL_CROSS
#define YorNC(y)  (y)? COLOR_y     : COLOR_n


#endif


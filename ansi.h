/**The configuration item
 * #define STB_ANSI_USE_UNICODE
 * #define STB_ANSI_NO_AUTO_DETECT - Disable automatic detection
 * #define STB_ANSI_FORCE_ENABLE   - Enforce ANSI
 * #define STB_ANSI_FORCE_DISABLE  - Force ANSI to be disabled
 * */

/**The Usage
 * #define STB_ansi
 * #include "ansi.h"
 * */

/*
 * ansi.h - ANSI转义序列修饰符库
 * 版本: 1.0
 * 
 * 用法:
 *   1. 在单个C文件中定义 STB_ansi
 *   2. 包含此头文件
 * 
 * 示例:
 *   #define STB_ansi
 *   #include "stb_ansi_modifier.h"
 * 
 * 注意: 默认自动检测ANSI支持，可通过宏控制
 *   STB_ANSI_NO_AUTO_DETECT - 禁用自动检测
 *   STB_ANSI_FORCE_ENABLE   - 强制启用ANSI
 *   STB_ANSI_FORCE_DISABLE  - 强制禁用ANSI
 */

#ifndef STB_ANSI_MODIFIER_INCLUDED
#define STB_ANSI_MODIFIER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/* ============================ 基本接口 ============================ */

/* 初始化ANSI支持检测 */
void stb_ansi_init(int force_enable);

/* 获取当前ANSI支持状态 */
int stb_ansi_enabled(void);

/* 启用/禁用ANSI输出 (不改变全局状态) */
void stb_ansi_set_enabled(int enabled);

/* 重置所有样式和颜色 */
#define ANSI_RESET "\033[0m"

/* ============================ 文本样式 ============================ */

/* 字体样式 */
#define ANSI_BOLD      "\033[1m"      /* 加粗 */
#define ANSI_DIM       "\033[2m"      /* 暗淡 */
#define ANSI_ITALIC    "\033[3m"      /* 斜体 */
#define ANSI_UNDERLINE "\033[4m"      /* 下划线 */
#define ANSI_BLINK     "\033[5m"      /* 闪烁 */
#define ANSI_REVERSE   "\033[7m"      /* 反色 */
#define ANSI_HIDDEN    "\033[8m"      /* 隐藏 */

/* 重置特定样式 */
#define ANSI_NO_BOLD      "\033[22m"
#define ANSI_NO_DIM       "\033[22m"
#define ANSI_NO_ITALIC    "\033[23m"
#define ANSI_NO_UNDERLINE "\033[24m"
#define ANSI_NO_BLINK     "\033[25m"
#define ANSI_NO_REVERSE   "\033[27m"
#define ANSI_NO_HIDDEN    "\033[28m"

/* ============================ 前景色 (8/16色) ============================ */

/* 标准颜色 */
#define ANSI_FG_BLACK   "\033[30m"
#define ANSI_FG_RED     "\033[31m"
#define ANSI_FG_GREEN   "\033[32m"
#define ANSI_FG_YELLOW  "\033[33m"
#define ANSI_FG_BLUE    "\033[34m"
#define ANSI_FG_MAGENTA "\033[35m"
#define ANSI_FG_CYAN    "\033[36m"
#define ANSI_FG_WHITE   "\033[37m"

/* 亮色 */
#define ANSI_FG_BLACK_BRIGHT   "\033[90m"
#define ANSI_FG_RED_BRIGHT     "\033[91m"
#define ANSI_FG_GREEN_BRIGHT   "\033[92m"
#define ANSI_FG_YELLOW_BRIGHT  "\033[93m"
#define ANSI_FG_BLUE_BRIGHT    "\033[94m"
#define ANSI_FG_MAGENTA_BRIGHT "\033[95m"
#define ANSI_FG_CYAN_BRIGHT    "\033[96m"
#define ANSI_FG_WHITE_BRIGHT   "\033[97m"

/* 重置前景色 */
#define ANSI_FG_DEFAULT "\033[39m"

/* ============================ 背景色 (8/16色) ============================ */

/* 标准背景色 */
#define ANSI_BG_BLACK   "\033[40m"
#define ANSI_BG_RED     "\033[41m"
#define ANSI_BG_GREEN   "\033[42m"
#define ANSI_BG_YELLOW  "\033[43m"
#define ANSI_BG_BLUE    "\033[44m"
#define ANSI_BG_MAGENTA "\033[45m"
#define ANSI_BG_CYAN    "\033[46m"
#define ANSI_BG_WHITE   "\033[47m"

/* 亮背景色 */
#define ANSI_BG_BLACK_BRIGHT   "\033[100m"
#define ANSI_BG_RED_BRIGHT     "\033[101m"
#define ANSI_BG_GREEN_BRIGHT   "\033[102m"
#define ANSI_BG_YELLOW_BRIGHT  "\033[103m"
#define ANSI_BG_BLUE_BRIGHT    "\033[104m"
#define ANSI_BG_MAGENTA_BRIGHT "\033[105m"
#define ANSI_BG_CYAN_BRIGHT    "\033[106m"
#define ANSI_BG_WHITE_BRIGHT   "\033[107m"

/* 重置背景色 */
#define ANSI_BG_DEFAULT "\033[49m"

/* ============================ 256色模式 ============================ */

/* 前景256色: \033[38;5;{n}m */
#define ANSI_FG_256(n) "\033[38;5;" #n "m"

/* 背景256色: \033[48;5;{n}m */
#define ANSI_BG_256(n) "\033[48;5;" #n "m"

/* ============================ 真彩色模式 ============================ */

/* RGB颜色: \033[38;2;{r};{g};{b}m */
#define ANSI_FG_RGB(r,g,b) "\033[38;2;" #r ";" #g ";" #b "m"

/* RGB背景: \033[48;2;{r};{g};{b}m */
#define ANSI_BG_RGB(r,g,b) "\033[48;2;" #r ";" #g ";" #b "m"

/* ============================ 光标控制 ============================ */

/* 光标移动 */
#define ANSI_CURSOR_UP(n)        "\033[" #n "A"
#define ANSI_CURSOR_DOWN(n)      "\033[" #n "B"
#define ANSI_CURSOR_RIGHT(n)     "\033[" #n "C"
#define ANSI_CURSOR_LEFT(n)      "\033[" #n "D"

/* 光标定位 */
#define ANSI_CURSOR_POS(row,col) "\033[" #row ";" #col "H"
#define ANSI_CURSOR_HOME         "\033[H"

/* 光标可见性 */
#define ANSI_CURSOR_SHOW         "\033[?25h"
#define ANSI_CURSOR_HIDE         "\033[?25l"

/* 保存/恢复光标位置 */
#define ANSI_CURSOR_SAVE         "\033[s"
#define ANSI_CURSOR_RESTORE      "\033[u"

/* ============================ 屏幕控制 ============================ */

/* 清屏 */
#define ANSI_CLEAR_SCREEN        "\033[2J"
#define ANSI_CLEAR_TO_END        "\033[0J"
#define ANSI_CLEAR_TO_START      "\033[1J"

/* 清行 */
#define ANSI_CLEAR_LINE          "\033[2K"
#define ANSI_CLEAR_TO_EOL        "\033[0K"
#define ANSI_CLEAR_TO_SOL        "\033[1K"

/* 滚动 */
#define ANSI_SCROLL_UP(n)        "\033[" #n "S"
#define ANSI_SCROLL_DOWN(n)      "\033[" #n "T"

/* ============================ 二值结果打印 ============================ */
/* Unicode符号（跨平台兼容） */
#define SYMBOL_TICK  "✓"  /* ✓ UTF-8 */
#define SYMBOL_CROSS "✗"  /* ✗ UTF-8 */

#ifdef STB_ANSI_USE_UNICODE

#define COLOR_y ANSI_FG_GREEN SYMBOL_TICK  ANSI_RESET
#define COLOR_n ANSI_FG_RED   SYMBOL_CROSS ANSI_RESET

#else

#define COLOR_y ANSI_FG_GREEN "[V]" ANSI_RESET
#define COLOR_n ANSI_FG_RED   "[X]" ANSI_RESET

#endif

#define YorN(y)   (y)? SYMBOL_TICK : SYMBOL_CROSS
#define YorNC(y)  (y)? COLOR_y     : COLOR_n

/* ============================ 高级功能 ============================ */

/* 函数接口 - 返回带ANSI修饰的文本（需手动free） */
char* stb_ansi_colorize(const char* text, const char* ansi_code);
char* stb_ansi_colorize_fg(const char* text, int r, int g, int b);
char* stb_ansi_colorize_bg(const char* text, int r, int g, int b);

/* 预设主题样式 */
const char* stb_ansi_success(const char* text);
const char* stb_ansi_error(const char* text);
const char* stb_ansi_warning(const char* text);
const char* stb_ansi_info(const char* text);
const char* stb_ansi_highlight(const char* text);
const char* stb_ansi_quote(const char* text);

/* 打印函数（自动处理ANSI状态） */
void stb_ansi_printf(const char* format, ...);
void stb_ansi_puts(const char* str);
void stb_ansi_print(const char* str);
void stb_ansi_println(const char* str);

/* 进度条和状态 */
void stb_ansi_progress_bar(int progress, int total, int width);
void stb_ansi_spinner(const char* label, int frame);
void stb_ansi_print_status(const char* message, int type);

/* 格式化工具 */
void stb_ansi_print_table(const char** headers, const char*** rows, 
                         int num_headers, int num_rows, int* col_widths);
void stb_ansi_print_boxed(const char* text, const char* style);
void stb_ansi_print_separator(int length, char ch, const char* color);

/* 缓存管理 */
void stb_ansi_push_state(void);
void stb_ansi_pop_state(void);
void stb_ansi_clear_cache(void);

#ifdef __cplusplus
}
#endif

#endif /* STB_ANSI_MODIFIER_INCLUDED */

/* ================================================================= */
/* 实现部分 */
#ifdef STB_ansi
#undef STB_ansi

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef STB_ANSI_NO_AUTO_DETECT
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define isatty _isatty
#else
#include <unistd.h>
#endif
#endif

/* 内部状态 */
static int stb_ansi_global_enabled = 0;
static int stb_ansi_initialized = 0;
static int stb_ansi_local_enabled = 1; /* 线程局部模拟 */

/* 内部缓冲区 */
#define STB_ANSI_BUFFER_SIZE 4096
static char stb_ansi_buffer[STB_ANSI_BUFFER_SIZE];

/* 状态栈 */
#define STB_ANSI_STACK_SIZE 10
static int stb_ansi_state_stack[STB_ANSI_STACK_SIZE];
static int stb_ansi_stack_top = 0;

/* 初始化ANSI支持检测 */
void stb_ansi_init(int force_enable)
{
    if (stb_ansi_initialized) return;
    
#ifdef STB_ANSI_FORCE_ENABLE
    stb_ansi_global_enabled = 1;
#elif defined(STB_ANSI_FORCE_DISABLE)
    stb_ansi_global_enabled = 0;
#else
#ifndef STB_ANSI_NO_AUTO_DETECT
    /* 自动检测 */
    int is_tty = 0;
    
#ifdef _WIN32
    /* Windows检测 */
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    is_tty = (GetConsoleMode(hConsole, &mode) != 0);
    
    if (is_tty) {
        /* Windows 10 Build 14393+ 支持ANSI */
        OSVERSIONINFOEX osvi = {0};
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        osvi.dwBuildNumber = 14393;
        
        DWORDLONG mask = 0;
        VER_SET_CONDITION(mask, VER_BUILDNUMBER, VER_GREATER_EQUAL);
        
        stb_ansi_global_enabled = VerifyVersionInfo(&osvi, VER_BUILDNUMBER, mask) != 0;
    } else {
        stb_ansi_global_enabled = force_enable;
    }
#else
    /* Unix-like系统检测 */
    is_tty = isatty(fileno(stdout));
    
    if (is_tty) {
        /* 检查终端类型 */
        const char* term = getenv("TERM");
        const char* colorterm = getenv("COLORTERM");
        
        stb_ansi_global_enabled = (term != NULL && (
            strstr(term, "xterm") != NULL ||
            strstr(term, "vt100") != NULL ||
            strstr(term, "color") != NULL ||
            strstr(term, "ansi") != NULL ||
            strstr(term, "linux") != NULL ||
            strcmp(term, "screen") == 0 ||
            strcmp(term, "tmux") == 0
        )) || (colorterm != NULL);
        
        /* 检查NO_COLOR环境变量 */
        if (getenv("NO_COLOR") != NULL) {
            stb_ansi_global_enabled = 0;
        }
    } else {
        stb_ansi_global_enabled = force_enable;
    }
#endif
#else
    stb_ansi_global_enabled = force_enable;
#endif
#endif
    
    stb_ansi_initialized = 1;
    stb_ansi_local_enabled = stb_ansi_global_enabled;
}

/* 获取ANSI支持状态 */
int stb_ansi_enabled(void)
{
    if (!stb_ansi_initialized) {
        stb_ansi_init(0);
    }
    return stb_ansi_global_enabled && stb_ansi_local_enabled;
}

/* 启用/禁用ANSI输出 */
void stb_ansi_set_enabled(int enabled)
{
    stb_ansi_local_enabled = enabled;
}

/* 应用ANSI代码到文本 */
static const char* apply_ansi(const char* ansi_code)
{
    if (stb_ansi_enabled()) {
        return ansi_code;
    }
    return "";
}

/* 颜色化文本 */
char* stb_ansi_colorize(const char* text, const char* ansi_code)
{
    if (!text) return NULL;
    
    if (!stb_ansi_enabled()) {
        return strdup(text);
    }
    
    int text_len = strlen(text);
    int ansi_len = strlen(ansi_code);
    int reset_len = strlen(ANSI_RESET);
    
    char* result = malloc(text_len + ansi_len + reset_len + 1);
    if (!result) return NULL;
    
    char* ptr = result;
    memcpy(ptr, ansi_code, ansi_len);
    ptr += ansi_len;
    memcpy(ptr, text, text_len);
    ptr += text_len;
    memcpy(ptr, ANSI_RESET, reset_len);
    ptr += reset_len;
    *ptr = '\0';
    
    return result;
}

/* RGB颜色化前景 */
char* stb_ansi_colorize_fg(const char* text, int r, int g, int b)
{
    if (!stb_ansi_enabled()) {
        return strdup(text);
    }
    
    char ansi_code[32];
    snprintf(ansi_code, sizeof(ansi_code), "\033[38;2;%d;%d;%dm", r, g, b);
    
    return stb_ansi_colorize(text, ansi_code);
}

/* RGB颜色化背景 */
char* stb_ansi_colorize_bg(const char* text, int r, int g, int b)
{
    if (!stb_ansi_enabled()) {
        return strdup(text);
    }
    
    char ansi_code[32];
    snprintf(ansi_code, sizeof(ansi_code), "\033[48;2;%d;%d;%dm", r, g, b);
    
    return stb_ansi_colorize(text, ansi_code);
}

/* 预设主题样式 */
const char* stb_ansi_success(const char* text)
{
    static char buffer[1024];
    if (!stb_ansi_enabled()) return text;
    
    snprintf(buffer, sizeof(buffer), "%s%s%s", 
             ANSI_FG_GREEN, text, ANSI_RESET);
    return buffer;
}

const char* stb_ansi_error(const char* text)
{
    static char buffer[1024];
    if (!stb_ansi_enabled()) return text;
    
    snprintf(buffer, sizeof(buffer), "%s%s%s", 
             ANSI_FG_RED, text, ANSI_RESET);
    return buffer;
}

const char* stb_ansi_warning(const char* text)
{
    static char buffer[1024];
    if (!stb_ansi_enabled()) return text;
    
    snprintf(buffer, sizeof(buffer), "%s%s%s", 
             ANSI_FG_YELLOW, text, ANSI_RESET);
    return buffer;
}

const char* stb_ansi_info(const char* text)
{
    static char buffer[1024];
    if (!stb_ansi_enabled()) return text;
    
    snprintf(buffer, sizeof(buffer), "%s%s%s", 
             ANSI_FG_CYAN, text, ANSI_RESET);
    return buffer;
}

const char* stb_ansi_highlight(const char* text)
{
    static char buffer[1024];
    if (!stb_ansi_enabled()) return text;
    
    snprintf(buffer, sizeof(buffer), "%s%s%s%s", 
             ANSI_BOLD, ANSI_FG_YELLOW, text, ANSI_RESET);
    return buffer;
}

const char* stb_ansi_quote(const char* text)
{
    static char buffer[1024];
    if (!stb_ansi_enabled()) return text;
    
    snprintf(buffer, sizeof(buffer), "%s%s%s", 
             ANSI_ITALIC ANSI_FG_CYAN, text, ANSI_RESET);
    return buffer;
}

/* 带ANSI支持的printf */
void stb_ansi_printf(const char* format, ...)
{
    if (!stb_ansi_enabled()) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        return;
    }
    
    va_list args;
    va_start(args, format);
    vsnprintf(stb_ansi_buffer, STB_ANSI_BUFFER_SIZE, format, args);
    va_end(args);
    
    printf("%s", stb_ansi_buffer);
}

/* 带ANSI支持的puts */
void stb_ansi_puts(const char* str)
{
    printf("%s\n", str);
}

/* 带ANSI支持的print */
void stb_ansi_print(const char* str)
{
    printf("%s", str);
}

/* 带ANSI支持的println */
void stb_ansi_println(const char* str)
{
    printf("%s\n", str);
}

/* 进度条 */
void stb_ansi_progress_bar(int progress, int total, int width)
{
    if (total == 0) return;
    
    float percent = (float)progress / total;
    int filled = (int)(percent * width);
    
    printf("\r[");
    
    /* 进度条主体 */
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            if (stb_ansi_enabled()) {
                /* 渐变颜色 */
                int r = (int)(255 * (1.0 - (float)i/width));
                int g = (int)(255 * (float)i/width);
                printf("\033[38;2;%d;%d;0m█\033[0m", r, g);
            } else {
                printf("#");
            }
        } else if (i == filled) {
            if (stb_ansi_enabled()) {
                printf(">");
            } else {
                printf(">");
            }
        } else {
            printf(" ");
        }
    }
    
    printf("] %3d%%", (int)(percent * 100));
    fflush(stdout);
}

/* 旋转指示器 */
void stb_ansi_spinner(const char* label, int frame)
{
    static const char* spinner_frames[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    static const char* ascii_frames[] = {"|", "/", "-", "\\"};
    
    int frame_count = stb_ansi_enabled() ? 10 : 4;
    int idx = frame % frame_count;
    const char* frame_char = stb_ansi_enabled() ? spinner_frames[idx] : ascii_frames[idx];
    
    if (stb_ansi_enabled()) {
        printf("\r%s %s%s%s", frame_char, ANSI_FG_CYAN, label, ANSI_RESET);
    } else {
        printf("\r%s %s", frame_char, label);
    }
    fflush(stdout);
}

/* 状态打印 */
void stb_ansi_print_status(const char* message, int type)
{
    /* 0: info, 1: success, 2: warning, 3: error */
    const char* prefix = "";
    const char* color = "";
    
    switch(type) {
        case 0: /* info */
            if (stb_ansi_enabled()) {
                prefix = "ℹ";
                color = ANSI_FG_CYAN;
            } else {
                prefix = "[INFO]";
            }
            break;
        case 1: /* success */
            if (stb_ansi_enabled()) {
                prefix = "✓";
                color = ANSI_FG_GREEN;
            } else {
                prefix = "[OK]";
            }
            break;
        case 2: /* warning */
            if (stb_ansi_enabled()) {
                prefix = "⚠";
                color = ANSI_FG_YELLOW;
            } else {
                prefix = "[WARN]";
            }
            break;
        case 3: /* error */
            if (stb_ansi_enabled()) {
                prefix = "✗";
                color = ANSI_FG_RED;
            } else {
                prefix = "[ERR]";
            }
            break;
    }
    
    if (stb_ansi_enabled()) {
        printf("%s%s %s%s\n", color, prefix, message, ANSI_RESET);
    } else {
        printf("%s %s\n", prefix, message);
    }
}

/* 打印表格 */
void stb_ansi_print_table(const char** headers, const char*** rows, 
                         int num_headers, int num_rows, int* col_widths)
{
    if (!headers || !rows || num_headers <= 0) return;
    
    /* 表头 */
    printf("┌");
    for (int i = 0; i < num_headers; i++) {
        int width = col_widths ? col_widths[i] : 20;
        for (int j = 0; j < width + 2; j++) {
            printf("─");
        }
        if (i < num_headers - 1) printf("┬");
    }
    printf("┐\n");
    
    printf("│");
    for (int i = 0; i < num_headers; i++) {
        int width = col_widths ? col_widths[i] : 20;
        if (stb_ansi_enabled()) {
            printf(" %s%-*s%s │", ANSI_BOLD, width, headers[i], ANSI_RESET);
        } else {
            printf(" %-*s │", width, headers[i]);
        }
    }
    printf("\n");
    
    /* 分隔线 */
    printf("├");
    for (int i = 0; i < num_headers; i++) {
        int width = col_widths ? col_widths[i] : 20;
        for (int j = 0; j < width + 2; j++) {
            printf("─");
        }
        if (i < num_headers - 1) printf("┼");
    }
    printf("┤\n");
    
    /* 数据行 */
    for (int r = 0; r < num_rows; r++) {
        printf("│");
        for (int c = 0; c < num_headers; c++) {
            int width = col_widths ? col_widths[c] : 20;
            const char* cell = rows[r][c];
            if (!cell) cell = "";
            
            if (stb_ansi_enabled() && r % 2 == 0) {
                printf(" %s%-*s%s │", ANSI_DIM, width, cell, ANSI_RESET);
            } else {
                printf(" %-*s │", width, cell);
            }
        }
        printf("\n");
    }
    
    /* 底部边框 */
    printf("└");
    for (int i = 0; i < num_headers; i++) {
        int width = col_widths ? col_widths[i] : 20;
        for (int j = 0; j < width + 2; j++) {
            printf("─");
        }
        if (i < num_headers - 1) printf("┴");
    }
    printf("┘\n");
}

/* 打印带框文本 */
void stb_ansi_print_boxed(const char* text, const char* style)
{
    int len = strlen(text);
    
    printf("┌");
    for (int i = 0; i < len + 2; i++) printf("─");
    printf("┐\n");
    
    if (stb_ansi_enabled()) {
        printf("│ %s%s%s │\n", style, text, ANSI_RESET);
    } else {
        printf("│ %s │\n", text);
    }
    
    printf("└");
    for (int i = 0; i < len + 2; i++) printf("─");
    printf("┘\n");
}

/* 打印分隔线 */
void stb_ansi_print_separator(int length, char ch, const char* color)
{
    if (stb_ansi_enabled()) {
        printf("%s", color);
    }
    
    for (int i = 0; i < length; i++) {
        printf("%c", ch);
    }
    
    if (stb_ansi_enabled()) {
        printf("%s\n", ANSI_RESET);
    } else {
        printf("\n");
    }
}

/* 状态管理 */
void stb_ansi_push_state(void)
{
    if (stb_ansi_stack_top < STB_ANSI_STACK_SIZE) {
        stb_ansi_state_stack[stb_ansi_stack_top++] = stb_ansi_local_enabled;
    }
}

void stb_ansi_pop_state(void)
{
    if (stb_ansi_stack_top > 0) {
        stb_ansi_local_enabled = stb_ansi_state_stack[--stb_ansi_stack_top];
    }
}

void stb_ansi_clear_cache(void)
{
    /* 清空缓冲区 */
    memset(stb_ansi_buffer, 0, STB_ANSI_BUFFER_SIZE);
    /* 重置状态栈 */
    stb_ansi_stack_top = 0;
}

#endif /* STB_ansi */

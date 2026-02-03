/* test_ansi.c - 使用示例 */
#define STB_ansi
#include "../../ansi.h"
#include <stdio.h>
#include <unistd.h>  /* 用于sleep */

int main(void)
{
    /* 自动初始化（检测ANSI支持） */
    stb_ansi_init(0);
    
    printf("ANSI支持: %s\n\n", stb_ansi_enabled() ? "启用" : "禁用");
    
    /* 1. 基本颜色 */
    printf("=== 基本颜色 ===\n");
    printf("%s红色%s ", ANSI_FG_RED, ANSI_RESET);
    printf("%s绿色%s ", ANSI_FG_GREEN, ANSI_RESET);
    printf("%s黄色%s ", ANSI_FG_YELLOW, ANSI_RESET);
    printf("%s蓝色%s\n", ANSI_FG_BLUE, ANSI_RESET);
    printf("%s洋红%s ", ANSI_FG_MAGENTA, ANSI_RESET);
    printf("%s青色%s ", ANSI_FG_CYAN, ANSI_RESET);
    printf("%s白色%s\n\n", ANSI_FG_WHITE, ANSI_RESET);
    
    /* 2. 亮色 */
    printf("=== 亮色 ===\n");
    printf("%s亮红%s ", ANSI_FG_RED_BRIGHT, ANSI_RESET);
    printf("%s亮绿%s ", ANSI_FG_GREEN_BRIGHT, ANSI_RESET);
    printf("%s亮黄%s ", ANSI_FG_YELLOW_BRIGHT, ANSI_RESET);
    printf("%s亮蓝%s\n\n", ANSI_FG_BLUE_BRIGHT, ANSI_RESET);
    
    /* 3. 背景色 */
    printf("=== 背景色 ===\n");
    printf("%s%s黑底白字%s ", ANSI_BG_BLACK, ANSI_FG_WHITE, ANSI_RESET);
    printf("%s%s红底白字%s\n", ANSI_BG_RED, ANSI_FG_WHITE, ANSI_RESET);
    
    /* 4. 文本样式 */
    printf("=== 文本样式 ===\n");
    printf("%s加粗%s ", ANSI_BOLD, ANSI_RESET);
    printf("%s下划线%s ", ANSI_UNDERLINE, ANSI_RESET);
    printf("%s斜体%s ", ANSI_ITALIC, ANSI_RESET);
    printf("%s反色%s\n\n", ANSI_REVERSE, ANSI_RESET);
    
    /* 5. 组合样式 */
    printf("=== 组合样式 ===\n");
    printf("%s%s错误信息%s\n", ANSI_BOLD, ANSI_FG_RED, ANSI_RESET);
    printf("%s%s警告信息%s\n", ANSI_BOLD, ANSI_FG_YELLOW, ANSI_RESET);
    printf("%s%s成功信息%s\n\n", ANSI_BOLD, ANSI_FG_GREEN, ANSI_RESET);
    
    /* 6. 256色 */
    printf("=== 256色示例 ===\n");

    for(int i = 0; i < 16; i += 1) {
    	printf(ANSI_FG_256(%d) "█" ANSI_RESET, i);
    }
    printf("\n\n");
    
    /* 7. 预设主题 */
    printf("=== 预设主题 ===\n");
    printf("%s\n", stb_ansi_success("操作成功！"));
    printf("%s\n", stb_ansi_error("发生错误！"));
    printf("%s\n", stb_ansi_warning("警告信息！"));
    printf("%s\n", stb_ansi_info("提示信息！"));
    printf("%s\n\n", stb_ansi_highlight("高亮文本！"));
    
    /* 8. 状态打印 */
    printf("=== 状态打印 ===\n");
    stb_ansi_print_status("系统初始化完成", 1);
    stb_ansi_print_status("磁盘空间不足", 2);
    stb_ansi_print_status("文件未找到", 3);
    stb_ansi_print_status("正在处理...", 0);
    printf("\n");
    
    /* 9. 进度条 */
    printf("=== 进度条 ===\n");
    for (int i = 0; i <= 100; i += 5) {
        stb_ansi_progress_bar(i, 100, 30);
        fflush(stdout);
        usleep(50000);  /* 50ms延迟 */
    }
    printf("\n\n");
    
    /* 10. 旋转指示器 */
    printf("=== 加载动画 ===\n");
    for (int i = 0; i < 20; i++) {
        stb_ansi_spinner("正在加载", i);
        fflush(stdout);
        usleep(100000);  /* 100ms延迟 */
    }
    printf("\n\n");
    
    /* 11. 表格 */
    printf("=== 表格示例 ===\n");
    const char* headers[] = {"ID", "名称", "价格", "库存"};
    const char* rows[3][4] = {
        {"001", "苹果", "$1.99", "100"},
        {"002", "香蕉", "$0.99", "50"},
        {"003", "橙子", "$2.49", "75"}
    };
    const char** row_ptrs[3] = {rows[0], rows[1], rows[2]};
    int col_widths[] = {6, 10, 8, 8};
    
    stb_ansi_print_table(headers, (const char***)row_ptrs, 4, 3, col_widths);
    printf("\n");
    
    /* 12. 带框文本 */
    printf("=== 带框文本 ===\n");
    stb_ansi_print_boxed("重要通知", ANSI_FG_YELLOW ANSI_BOLD);
    printf("\n");
    
    /* 13. 分隔线 */
    stb_ansi_print_separator(50, '=', ANSI_FG_CYAN);
    
    /* 14. RGB颜色 */
    printf("=== RGB颜色 ===\n");
    for (int r = 0; r <= 255; r += 51) {
        for (int g = 0; g <= 255; g += 51) {
            for (int b = 0; b <= 255; b += 51) {
                printf(ANSI_FG_RGB(%d,%d,%d) "█" ANSI_RESET, r,g,b);
                if (b == 255) break;
            }
        }
        printf("\n");
    }
    
    return 0;
}

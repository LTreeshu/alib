/**The configuration item
 * #define TSysTickCnt - the SysTick count
 * #define TSystemCoreClock - Disable automatic detection
 * #define TSysTick_open() - open and start systick
 * #define TickTIMx_MAX
 * #define USE_TimerWheel
 * */

/**The Usage
 * configuration item
 * ...
 * #define STB_task
 * #include "task.h"
 * */

#ifndef _TASK_H_
#define _TASK_H_
#include <stdint.h>
#include <ch32v00x.h>

#ifndef TSysTickCnt
#error You must first define the SysTick count to use this component.
#endif

#ifndef TSystemCoreClock
#error You must first define the SystemCoreClock value to use this component.
#endif

#ifndef TSysTick_open /* TSysTick_open() */
#error You must first define the SysTick open funcation to use this component.
#endif

#ifndef TickTIMx_MAX
#define TickTIMx_MAX 8
#endif

#define __TaskInit(task, ticks, call)                            \
    task_t task = {.period = ticks, .compare = 0, .proc = call}; \
    task.compare = task.period + TSysTickCnt;                   \
    TaskAdd (&task);

typedef struct list_node lnode_t;

struct list_node {
    lnode_t *next;
};
lnode_t * list_search(lnode_t* node);

typedef struct task {
    struct task *next;
    void (*proc)();
    uint32_t period;
    uint32_t compare;
    uint32_t overflow;
} task_t;

typedef union {
    struct {
        uint32_t s: 6;
        uint32_t m: 6;
        uint32_t h: 20;
    };
    uint32_t value;
} twheel_t;

typedef void (*callback_t)(void);
typedef struct twtimer {
    struct twtimer* next;
    struct twtimer* prev;
    callback_t callback;
    uint32_t period;
    uint32_t compare;
    uint32_t overflow;
    uint32_t cycle;
#define TTIM_Once  1
#define TTIM_Cycle 0
} twtimer_t;

#define TASK_NULL  (task_t){NULL,NULL,0,0,0}
#define TWTIM_NULL (twtimer_t){NULL,NULL,NULL,0,0,0,0}

static uint32_t  p_us                    = 0U;
static uint32_t  p_ms                    = 0U;
static twheel_t  twheel                  = {0U, 0U, 0U};
static task_t    taskHead                = TASK_NULL;
static twtimer_t timerHead               = TWTIM_NULL;
static twtimer_t timerPool[TickTIMx_MAX] = {0};

void TaskManager_Init (void);
void TaskAdd (task_t *pt);
void TaskDelete(task_t *pt);
void TaskRun(void);
/* TwtimerInit() */
void TwtimerAdd(twheel_t tm, uint32_t cycle, callback_t cb);
void TwtimerDelete(twtimer_t *twt);
void TaskTwheelInit(void);
void twheel_entry(void);

uint32_t Get_UsTicks (uint32_t n) { return n * p_ms; }

uint32_t Get_MsTicks (uint32_t n) { return n * p_ms; }

uint32_t Get_sTicks (uint32_t n) { return n * p_ms * 1000U; }

void TaskManager_Init (void)
{
    p_us = TSystemCoreClock / 8000000;
    p_ms = (uint16_t)p_us * 1000;
    TSysTick_open();
    TaskTwheelInit();
}

void TaskInit(task_t *pt)
{
    pt->compare = pt->period + TSysTickCnt;
    TaskAdd (pt);
}

void TaskAdd (task_t *pt)
{
    task_t *node = pt;

    /* 避免重复添加 */
    if (taskHead.next == node) {
        return;
    }

    pt->next = taskHead.next;
    taskHead.next = node;
}

void TaskDelete(task_t *pt)
{
    task_t *prev = NULL;
    task_t *search = &taskHead;
    while (search->next != pt)
    {
        search = search->next;
    }

    prev = search;

    if (prev == NULL) {return;}

    prev->next = pt->next;
    pt->next = NULL;
}

void TaskRun(void)
{
    task_t *pt = taskHead.next;

    while (pt != NULL && pt->proc != NULL)
    {

        if (TSysTickCnt < pt->overflow) {
            pt->overflow = 0;
        }

        if (pt->compare <= TSysTickCnt && pt->overflow == 0) {
            pt->proc();
            uint32_t compare = TSysTickCnt + pt->period;

            if (compare < pt->compare) {
                pt->overflow = pt->compare;
            }

            pt->compare = compare;
        }

        pt = pt->next;
    }
}

void TwtimerAdd(twheel_t tm, uint32_t cycle,  callback_t cb)
{
    static uint32_t used = 0;

    if (used > 9) {
        uint8_t i = 9;
        while (i--)
        {
            if (timerPool[i].period == 0) {
                used = i;
                break;
            }
        }

        if (i != used) return;
    }

    twtimer_t* node = &timerPool[used];

    /* 避免重复添加 */
    if (timerHead.next == node) {
        return;
    }

    node->period = tm.value;
    node->callback = cb;
    node->compare = tm.value + twheel.value;
    node->cycle = cycle;

    node->next = timerHead.next;
    timerHead.next = node;
    node->prev = &timerHead;

    used += 1;
}

void TwtimerDelete(twtimer_t *twt)
{
    /* 重复删除判断 */
    if (twt->prev == NULL) return;
    twt->prev->next = twt->next;

    /* 尾节点判断 */
    if (twt->next == NULL) return;
    twt->next->prev = twt->prev;

    /* 清理 */
    twt->next = NULL;
    twt->prev = NULL;
    *twt = (twtimer_t) {0, 0, 0, 0, 0, 0};
}

void TaskTwheelInit(void)
{
#ifdef USE_TimerWheel
    static task_t twheel_task = {.proc = twheel_entry};
    TaskAdd(&twheel_task);
    twheel_task.period = Get_sTicks(1);
    twheel_task.compare = twheel_task.period + TSysTickCnt;
}

/* period 937500 us*/
void twheel_entry(void)
{
    twheel.s += 1;
    if (twheel.s == 60) {
        twheel.s = 0;
        twheel.m += 1;
    }

    if (twheel.m == 60) {
        twheel.m = 0;
        twheel.h += 1;
    }

    twtimer_t* ptt = &timerHead;

    while (ptt = ptt->next, ptt != NULL )
    {
        ptt->overflow = (twheel.value < ptt->overflow) ? ptt->overflow : 0;
        if (ptt->compare <= twheel.value && ptt->overflow == 0) {
            ptt->callback();

            uint32_t compare = twheel.value + ptt->period;
            ptt->overflow = (compare < ptt->compare) ? ptt->compare : 0;
            ptt->compare = compare;

            if (ptt->cycle == 1) {
                ptt->prev->next = ptt->next;
                ptt->next->prev = ptt->prev;
                /* clean */
                *ptt = TWTIM_NULL;
            }
            else if (ptt->cycle > 1) {
                ptt->cycle -= 1;
            }

        }
    }
}
#else /*  USE_TimerWheel */
}
#endif /*  USE_TimerWheel */

uint32_t Get_twheelValue(void) { return twheel.value; }

twheel_t Get_twheelTime(void) { return twheel; }

char* uint2str(uint64_t value, char* buffer)
{
    // 边界处理：直接处理 0 的情况
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    // 1. 使用 __builtin_clzll 快速确定位数（基于二进制位数映射）

    int digit_count = (((64 - __builtin_clzll(value)) * 1233) >> 12) + 1;

    // 2. 创建一个临时缓冲区反向写入
    char temp[21];
    int pos = digit_count; // 从最高位开始填充
    temp[pos] = '\0';      // 设置字符串终止符

    // 3. 根据位数选择处理策略
    // 如果位数较多（>= 4 位），我们使用除以 100 的方式，一次处理两位
    // 这样可以将循环次数减少一半（例如 12 位数字只需要 6 次循环）
    while (digit_count >= 4) {
        // 取模 100，得到当前最低的两位数字
        uint64_t two_digits = value % 100;
        // 整除 100，去掉最低的两位
        value /= 100;

        // 处理两位数字
        // 由于不使用查表，需要分别处理个位和十位
        uint8_t low = two_digits % 10;  // 个位
        uint8_t high = two_digits / 10; // 十位

        // 逆序写入字符
        temp[--pos] = (char)(low + 0x30);  // 个位字符
        temp[--pos] = (char)(high + 0x30); // 十位字符

        digit_count -= 2; // 处理了两位
    }

    // 处理剩余的 1~3 位数字
    while (digit_count > 0) {
        // 对于最后几位，直接使用加 0x30 的方式（效率最高）
        uint64_t digit = value % 10;
        value /= 10;
        temp[--pos] = (char)(digit + 0x30);
        digit_count--;
    }

    // 4. 将结果拷贝到输出缓冲区
    char *dst = buffer;
    char *src = temp;
    while (*src) {
        *dst++ = *src++;
    }
    *dst = '\0';
    return buffer;
}

const char* Get_twheelTimeStr(void)
{
    static char str[] = "xxxxxxxxxx:xx:xx";
    
    uint2str(twheel.h, str);
    uint2str(twheel.m, &str[11]);
    uint2str(twheel.s, &str[14]);

    return str;
}


#endif /* _TASK_H_ */
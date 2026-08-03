/**The configuration item
 * 配置项
 * #define TSysTickCnt - the SysTick count  // SysTick计数值
 * #define TSystemCoreClock - Disable automatic detection  // 禁用自动检测（未使用）
 * #define TSysTick_open() - open and start systick  // 打开并启动systick
 * #define TickTIMx_MAX
 * #define USE_TimerWheel
 * */

/**The Usage
 * 使用方法
 * configuration item  // 配置项
 * ...
 * #define STB_task
 * #include "task.h"
 * */

#ifndef _TASK_H_
#define _TASK_H_
#include <stdint.h>
#include <stdio.h>

#ifndef TSysTickCnt
#error You must first define the SysTick count to use this component.
#endif

#ifndef TSysTickMAX
#error You must first define the Max SysTick count to use this component.
#endif

#ifndef TUsCnt
#error You must first define the SystemCoreClock value to use this component.
#endif

#ifndef TSysTick_open /* TSysTick_open() */
#error You must first define the SysTick open funcation to use this component.
#endif

#ifndef TickTIMx_MAX
#define TickTIMx_MAX 8
#endif

#define TWHEEL_TIME_STR_SIZE (21)

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
struct {
    uint32_t free;
    uint32_t used;
    twtimer_t* pool[TickTIMx_MAX];
    size_t capacity;
} twtmgr = {.capacity = sizeof(twtmgr.pool) / sizeof(twtmgr.pool[0])};

void TaskManager_Init (void);
void TaskAdd (task_t *pt);
void TaskDelete(task_t *pt);
void TaskRun(void);
/* TwtimerInit() */
void TwtimerAdd(twheel_t tm, uint32_t cycle, callback_t cb);
void TwtimerDelete(twtimer_t *twt);
void TaskTwheelInit(void);
void twheel_entry(void);
uint8_t timerPoolalloc(twtimer_t **tw);
void TDelay_Us(uint32_t n);
void TDelay_Ms(uint32_t n);

uint32_t Get_UsTicks (uint32_t n) { return n * p_us; }

uint32_t Get_MsTicks (uint32_t n) { return n * p_ms; }

uint32_t Get_sTicks (uint32_t n) { return n * p_ms * 1000U; }

void TaskManager_Init (void)
{
    p_us = TUsCnt;
    p_ms = p_us * 1000;
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

    /* 避免重复添加 */  // Avoid duplicate addition
    if (taskHead.next == node) {
        return;
    }

    pt->next = taskHead.next;
    taskHead.next = node;
}

void TaskDelete(task_t *pt)
{
    if (pt == NULL) return;

    task_t *prev = &taskHead;

    while (prev->next != NULL && prev->next != pt)
    {
        prev = prev->next;
    }

    if (prev->next == NULL) return;

    prev->next = pt->next;
    pt->next = NULL;
}

void TaskRun(void)
{
    task_t *pt = taskHead.next;

    register uint32_t syscnt;

    while (pt != NULL && pt->proc != NULL)
    {
        syscnt = TSysTickCnt;

        if (syscnt < pt->overflow) {
            pt->overflow = 0;
        }

        if (pt->compare <= syscnt && pt->overflow == 0) {
            pt->proc();
            uint32_t compare = syscnt + pt->period;
            compare %= TSysTickMAX;

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
    twtimer_t* node = NULL;

    if (twtmgr.used == twtmgr.free) return;

    node = twtmgr.pool[twtmgr.used++];
    twtmgr.used %= twtmgr.capacity;

    if (node == NULL) return;


    /* 避免重复添加 */  // Avoid duplicate addition
    if (timerHead.next == node) return;


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
    /* 重复删除判断 */  // Duplicate deletion check
    if (twt->prev == NULL) return;
    twt->prev->next = twt->next;

    /* 尾节点判断 */  // Tail node check
    if (twt->next == NULL) return;
    twt->next->prev = twt->prev;

    /* 清理 */  // Cleanup
    twt->next = NULL;
    twt->prev = NULL;
    *twt = (twtimer_t) {0, 0, 0, 0, 0, 0};

    if ((twtmgr.free + 1) % twtmgr.capacity == twtmgr.used) {
        return ;
    }

    twtmgr.pool[twtmgr.free++] = twt;
    twtmgr.free %= twtmgr.capacity;
}

void TaskTwheelInit(void)
{
#ifdef USE_TimerWheel
    static task_t twheel_task = {.proc = twheel_entry};

    // 初始化软定时器内存池  // Initialize soft timer memory pool
    twtmgr.free = 0;
    twtmgr.used = 0;
    for (int i = 0; i < twtmgr.capacity; i++) {

        twtmgr.pool[twtmgr.free] = &timerPool[i];

        if ((twtmgr.free + 1) % twtmgr.capacity == twtmgr.used) break;
        twtmgr.free = (twtmgr.free + 1) % twtmgr.capacity;
    }

    TaskAdd(&twheel_task);
    twheel_task.period = Get_MsTicks(1);
    twheel_task.compare = twheel_task.period + TSysTickCnt;
}

/* period 937500 us*/
void twheel_entry(void)
{
    static uint32_t msCnt = 0U;

    msCnt += 1U;
    if (msCnt == 1000U) {
        twheel.s += 1U;
        msCnt = 0;
    }

    if (twheel.s == 60U) {
        twheel.s = 0U;
        twheel.m += 1U;
    }

    if (twheel.m == 60U) {
        twheel.m = 0U;
        twheel.h += 1U;
    }

    twtimer_t* ptt = &timerHead;

    while (ptt = ptt->next, ptt != NULL )
    {
        ptt->overflow = (twheel.value < ptt->overflow) ? ptt->overflow : 0U;
        if (ptt->compare <= twheel.value && ptt->overflow == 0U) {
            ptt->callback();

            uint32_t compare = twheel.value + ptt->period;
            ptt->overflow = (compare < ptt->compare) ? ptt->compare : 0U;
            ptt->compare = compare;

            if (ptt->cycle == 1U) {
                TwtimerDelete(ptt);
                /* clean */  /* 清理 */
                *ptt = TWTIM_NULL;
            }
            else if ( ptt -> cycle == 0U) {;}
            else {
                ptt->cycle -= 1U;
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
    // 边界处理：直接处理 0 的情况  // Boundary handling: handle 0 directly
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    // 1. 使用 __builtin_clzll 快速确定位数（基于二进制位数映射）  // Use __builtin_clzll to quickly determine digit count (based on binary length mapping)
    int digit_count = (((64 - __builtin_clzll(value)) * 1233) >> 12) + 1;

    // 2. 创建一个临时缓冲区反向写入  // Create temporary buffer for reverse writing
    char temp[21];
    int pos = digit_count; // 从最高位开始填充  // Start filling from most significant digit
    temp[pos] = '\0';      // 设置字符串终止符  // Set string terminator

    // 3. 根据位数选择处理策略  // Choose processing strategy based on digit count
    // 如果位数较多（>= 4 位），我们使用除以 100 的方式，一次处理两位  // If digits >= 4, use divide by 100 to process two digits at a time
    // 这样可以将循环次数减少一半（例如 12 位数字只需要 6 次循环）  // This halves loop count (e.g., 12 digits -> 6 iterations)
    while (digit_count >= 4) {
        // 取模 100，得到当前最低的两位数字  // Mod 100 to get lowest two digits
        uint64_t two_digits = value % 100;
        // 整除 100，去掉最低的两位  // Divide by 100 to remove lowest two digits
        value /= 100;

        // 处理两位数字  // Process two digits
        // 由于不使用查表，需要分别处理个位和十位  // Without lookup table, handle units and tens separately
        uint8_t low = two_digits % 10;  // 个位  // units
        uint8_t high = two_digits / 10; // 十位  // tens

        // 逆序写入字符  // Write characters in reverse order
        temp[--pos] = (char)(low + 0x30);  // 个位字符  // units char
        temp[--pos] = (char)(high + 0x30); // 十位字符  // tens char

        digit_count -= 2; // 处理了两位  // processed two digits
    }

    // 处理剩余的 1~3 位数字  // Process remaining 1~3 digits
    while (digit_count > 0) {
        // 对于最后几位，直接使用加 0x30 的方式（效率最高）  // For last digits, use +0x30 (most efficient)
        uint64_t digit = value % 10;
        value /= 10;
        temp[--pos] = (char)(digit + 0x30);
        digit_count--;
    }

    // 4. 将结果拷贝到输出缓冲区  // Copy result to output buffer
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
    const char zero[TWHEEL_TIME_STR_SIZE] = {0};
    static char str[TWHEEL_TIME_STR_SIZE] = {0};

    typedef struct {char _[TWHEEL_TIME_STR_SIZE];}*pcopy;
    *((pcopy)str) = *((pcopy)zero);

    uint8_t index = 0;

    uint2str(twheel.h, str);
    while (str[index] != '\0') index++;
    str[index++] = ':';

    uint2str(twheel.m, &str[index]);
    while (str[index] != '\0') index++;
    str[index++] = ':';

    uint2str(twheel.s, &str[index]);

    return str;
}

/*********************************************************************
 * @fn      TDelay_Us
 *
 * @brief   Microsecond Delay Time.  // 微秒延时
 *
 * @param   n - Microsecond number.  // 微秒数
 *
 * @return  None
 */
void TDelay_Us(uint32_t n)
{
    uint32_t ticks = n * p_us;
    uint32_t now = 0;
    uint32_t elapsed = 0;
    uint32_t start = TSysTickCnt;

    do {
        now = TSysTickCnt;
        if (now > start) {
            elapsed = now - start;
        }
        else {
            elapsed = (TSysTickMAX - start) + now + 1;
        }
    } while (elapsed < ticks);
}

/*********************************************************************
 * @fn      TDelay_Ms
 *
 * @brief   Millisecond Delay Time.  // 毫秒延时
 *
 * @param   n - Millisecond number.  // 毫秒数
 *
 * @return  None
 */
void TDelay_Ms(uint32_t n)
{
    uint32_t ticks = n * p_ms;
    uint32_t now = 0;
    uint32_t elapsed = 0;
    uint32_t start = TSysTickCnt;

    do {
        now = TSysTickCnt;
        if (now > start) {
            elapsed = now - start;
        }
        else {
            elapsed = (TSysTickMAX - start) + now + 1;
        }
    } while (elapsed < ticks);
}

#endif /* _TASK_H_ */
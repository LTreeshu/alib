// fsm.h - 静态状态机库 (STB风格)
// 版本: 1.1 (精简版)
// 特性: 零依赖, 静态分配, 事件驱动, MCU友好
// 精简点(v1.1): 去除 name 字符串/FSM_DEBUG/冗余API(callback_e, force_state,
//               reset, current_state_name), MAX_STATES 16->8, 删除未用 MAX_EVENTS

#ifndef FSM_H
#define FSM_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ================== 配置宏 ==================
#ifndef FSM_MAX_STATES
#define FSM_MAX_STATES      8       // 最大状态数
#endif

// ================== 类型定义 ==================
typedef uint8_t fsm_state_id_t;    // 状态ID类型
typedef uint8_t fsm_event_id_t;    // 事件ID类型
typedef void (*fsm_action_t)(void* ctx);  // 动作回调函数类型
typedef bool (*fsm_guard_t)(void* ctx);   // 守卫条件回调函数类型

// 状态结构体
typedef struct {
    fsm_action_t on_enter;          // 进入状态回调
    fsm_action_t on_exit;           // 退出状态回调
    fsm_action_t on_stay;           // 停留状态回调(处理事件但不转移)
} fsm_state_t;

// 转移结构体
typedef struct {
    fsm_state_id_t from;            // 源状态
    fsm_event_id_t event;           // 触发事件
    fsm_state_id_t to;              // 目标状态
    fsm_action_t action;            // 转移动作
    fsm_guard_t guard;              // 守卫条件(可选)
} fsm_transition_t;

// 状态机结构体
typedef struct {
    const fsm_state_t* states[FSM_MAX_STATES];  // 状态表
    const fsm_transition_t* transitions;         // 转移表
    uint8_t transition_count;                    // 转移数量
    fsm_state_id_t current;                      // 当前状态
    fsm_state_id_t initial;                      // 初始状态
    void* context;                               // 用户上下文
    bool initialized;                            // 初始化标志
} fsm_t;

// ================== 宏定义工具 ==================
// 定义状态（_name 仅文档用途, v1.1 起不存储, 兼容旧 4 参调用）
#define FSM_STATE_DEF(_name, _on_enter, _on_exit, _on_stay) \
    &(fsm_state_t){ \
        .on_enter = _on_enter, \
        .on_exit = _on_exit, \
        .on_stay = _on_stay \
    }

// 定义转移
#define FSM_TRANSITION_DEF(_from, _event, _to, _action, _guard) \
    (fsm_transition_t){ \
        .from = _from, \
        .event = _event, \
        .to = _to, \
        .action = _action, \
        .guard = _guard \
    }

// 声明状态机
#define FSM_DEF(_name, _initial) \
    static fsm_t _name = { \
        .current = _initial, \
        .initial = _initial, \
        .initialized = false \
    }

// ================== API函数 ==================
/**
 * @brief 初始化状态机
 */
static inline void fsm_init(fsm_t* fsm,
                           const fsm_state_t** states,
                           uint8_t state_count,
                           const fsm_transition_t* transitions,
                           uint8_t transition_count,
                           void* context) {
    if (!fsm || !states || state_count == 0) return;

    for (uint8_t i = 0; i < state_count && i < FSM_MAX_STATES; i++) {
        fsm->states[i] = states[i];
    }

    fsm->transitions = transitions;
    fsm->transition_count = transition_count;
    fsm->context = context;
    fsm->current = fsm->initial;
    fsm->initialized = true;

    if (fsm->states[fsm->current] && fsm->states[fsm->current]->on_enter) {
        fsm->states[fsm->current]->on_enter(context);
    }
}

/**
 * @brief 触发事件
 * @return 是否发生状态转移
 */
static inline bool fsm_dispatch(fsm_t* fsm, fsm_event_id_t event) {
    if (!fsm || !fsm->initialized) return false;

    const fsm_state_t* current_state = fsm->states[fsm->current];
    if (!current_state) return false;

    for (uint8_t i = 0; i < fsm->transition_count; i++) {
        const fsm_transition_t* trans = &fsm->transitions[i];

        if (trans->from == fsm->current && trans->event == event) {
            if (trans->guard && !trans->guard(fsm->context)) {
                continue;  // 守卫条件不满足，跳过此转移
            }

            if (current_state->on_exit) {
                current_state->on_exit(fsm->context);
            }
            if (trans->action) {
                trans->action(fsm->context);
            }
            fsm->current = trans->to;

            const fsm_state_t* new_state = fsm->states[fsm->current];
            if (new_state && new_state->on_enter) {
                new_state->on_enter(fsm->context);
            }
            return true;
        }
    }

    if (current_state->on_stay) {
        current_state->on_stay(fsm->context);
    }
    return false;
}

/**
 * @brief 获取当前状态ID
 */
static inline fsm_state_id_t fsm_current_state(const fsm_t* fsm) {
    return fsm ? fsm->current : 0;
}

#ifdef __cplusplus
}
#endif

#endif // FSM_H

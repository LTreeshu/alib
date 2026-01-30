/**deque.h configuration item
 * #define DEQUE_VALUE_TYPE int      // 默认int
 * #define DEQUE_INITIAL_CAPACITY 16 // 默认初始容量
 * #define DEQUE_ASSERT(x)          // 自定义断言
 * #define DEQUE_MALLOC(size)       // 自定义内存分配
 * #define DEQUE_FREE(ptr)          // 自定义内存释放
 * */

/**The Usage
 * #define STB_deque
 * #include "deque.h"
 * */

#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h>
#include <stdbool.h>

#ifndef DEQUE_VALUE_TYPE
#define DEQUE_VALUE_TYPE int
#endif

#ifndef DEQUE_INITIAL_CAPACITY
#define DEQUE_INITIAL_CAPACITY 16
#endif

#ifndef DEQUE_ASSERT
#include <assert.h>
#define DEQUE_ASSERT(x) assert(x)
#endif

#ifndef DEQUE_MALLOC
#include <stdlib.h>
#define DEQUE_MALLOC(size) malloc(size)
#define DEQUE_FREE(ptr) free(ptr)
#endif

typedef struct {
    DEQUE_VALUE_TYPE* data;
    size_t front;
    size_t rear;
    size_t size;
    size_t capacity;
    bool is_empty;
} deque;

// 函数声明
deque* deque_create(void);
void deque_destroy(deque* dq);
bool deque_push_front(deque* dq, DEQUE_VALUE_TYPE value);
bool deque_push_back(deque* dq, DEQUE_VALUE_TYPE value);
DEQUE_VALUE_TYPE deque_pop_front(deque* dq);
DEQUE_VALUE_TYPE deque_pop_back(deque* dq);
DEQUE_VALUE_TYPE deque_front(const deque* dq);
DEQUE_VALUE_TYPE deque_back(const deque* dq);
bool deque_empty(const deque* dq);
size_t deque_size(const deque* dq);
void deque_clear(deque* dq);
bool deque_reserve(deque* dq, size_t new_capacity);

// 迭代器支持
typedef struct {
    const deque* dq;
    size_t current;
    size_t count;
} deque_iterator;

deque_iterator deque_begin(const deque* dq);
deque_iterator deque_end(const deque* dq);
bool deque_iterator_equal(deque_iterator a, deque_iterator b);
void deque_iterator_next(deque_iterator* it);
DEQUE_VALUE_TYPE deque_iterator_value(const deque_iterator* it);

#endif // DEQUE_H

#ifdef STB_deque

deque* deque_create(void) {
    deque* dq = (deque*)DEQUE_MALLOC(sizeof(deque));
    if (!dq) return NULL;
    
    dq->data = (DEQUE_VALUE_TYPE*)DEQUE_MALLOC(DEQUE_INITIAL_CAPACITY * sizeof(DEQUE_VALUE_TYPE));
    if (!dq->data) {
        DEQUE_FREE(dq);
        return NULL;
    }
    
    dq->front = 0;
    dq->rear = 0;
    dq->size = 0;
    dq->capacity = DEQUE_INITIAL_CAPACITY;
    dq->is_empty = true;
    return dq;
}

void deque_destroy(deque* dq) {
    if (dq) {
        DEQUE_FREE(dq->data);
        DEQUE_FREE(dq);
    }
}

static bool deque_resize(deque* dq, size_t new_capacity) {
    DEQUE_VALUE_TYPE* new_data = (DEQUE_VALUE_TYPE*)DEQUE_MALLOC(new_capacity * sizeof(DEQUE_VALUE_TYPE));
    if (!new_data) return false;
    
    if (!dq->is_empty) {
        if (dq->front < dq->rear) {
            // 连续存储的情况
            for (size_t i = 0; i < dq->size; ++i) {
                new_data[i] = dq->data[dq->front + i];
            }
        } else {
            // 环绕存储的情况
            size_t first_part = dq->capacity - dq->front;
            for (size_t i = 0; i < first_part; ++i) {
                new_data[i] = dq->data[dq->front + i];
            }
            for (size_t i = 0; i < dq->rear; ++i) {
                new_data[first_part + i] = dq->data[i];
            }
        }
    }
    
    DEQUE_FREE(dq->data);
    dq->data = new_data;
    dq->front = 0;
    dq->rear = dq->size;
    dq->capacity = new_capacity;
    return true;
}

bool deque_push_front(deque* dq, DEQUE_VALUE_TYPE value) {
    DEQUE_ASSERT(dq);
    
    if (dq->size == dq->capacity) {
        if (!deque_resize(dq, dq->capacity * 2)) {
            return false;
        }
    }
    
    if (dq->is_empty) {
        dq->data[dq->front] = value;
        dq->is_empty = false;
    } else {
        dq->front = (dq->front == 0) ? dq->capacity - 1 : dq->front - 1;
        dq->data[dq->front] = value;
    }
    dq->size++;
    return true;
}

bool deque_push_back(deque* dq, DEQUE_VALUE_TYPE value) {
    DEQUE_ASSERT(dq);
    
    if (dq->size == dq->capacity) {
        if (!deque_resize(dq, dq->capacity * 2)) {
            return false;
        }
    }
    
    if (dq->is_empty) {
        dq->data[dq->rear] = value;
        dq->is_empty = false;
    } else {
        dq->rear = (dq->rear + 1) % dq->capacity;
        dq->data[dq->rear] = value;
    }
    dq->size++;
    return true;
}

DEQUE_VALUE_TYPE deque_pop_front(deque* dq) {
    DEQUE_ASSERT(dq && !dq->is_empty);
    
    DEQUE_VALUE_TYPE value = dq->data[dq->front];
    dq->size--;
    
    if (dq->size == 0) {
        dq->is_empty = true;
    } else {
        dq->front = (dq->front + 1) % dq->capacity;
    }
    return value;
}

DEQUE_VALUE_TYPE deque_pop_back(deque* dq) {
    DEQUE_ASSERT(dq && !dq->is_empty);
    
    DEQUE_VALUE_TYPE value = dq->data[dq->rear];
    dq->size--;
    
    if (dq->size == 0) {
        dq->is_empty = true;
    } else {
        dq->rear = (dq->rear == 0) ? dq->capacity - 1 : dq->rear - 1;
    }
    return value;
}

DEQUE_VALUE_TYPE deque_front(const deque* dq) {
    DEQUE_ASSERT(dq && !dq->is_empty);
    return dq->data[dq->front];
}

DEQUE_VALUE_TYPE deque_back(const deque* dq) {
    DEQUE_ASSERT(dq && !dq->is_empty);
    return dq->data[dq->rear];
}

bool deque_empty(const deque* dq) {
    DEQUE_ASSERT(dq);
    return dq->is_empty;
}

size_t deque_size(const deque* dq) {
    DEQUE_ASSERT(dq);
    return dq->size;
}

void deque_clear(deque* dq) {
    DEQUE_ASSERT(dq);
    dq->front = 0;
    dq->rear = 0;
    dq->size = 0;
    dq->is_empty = true;
}

bool deque_reserve(deque* dq, size_t new_capacity) {
    DEQUE_ASSERT(dq);
    if (new_capacity <= dq->capacity) return true;
    return deque_resize(dq, new_capacity);
}

// 迭代器实现
deque_iterator deque_begin(const deque* dq) {
    DEQUE_ASSERT(dq);
    deque_iterator it = {dq, dq->front, 0};
    return it;
}

deque_iterator deque_end(const deque* dq) {
    DEQUE_ASSERT(dq);
    deque_iterator it = {dq, 0, dq->size};
    return it;
}

bool deque_iterator_equal(deque_iterator a, deque_iterator b) {
    return a.dq == b.dq && a.count == b.count;
}

void deque_iterator_next(deque_iterator* it) {
    DEQUE_ASSERT(it && it->dq);
    if (it->count < it->dq->size) {
        it->current = (it->current + 1) % it->dq->capacity;
        it->count++;
    }
}

DEQUE_VALUE_TYPE deque_iterator_value(const deque_iterator* it) {
    DEQUE_ASSERT(it && it->dq && it->count < it->dq->size);
    return it->dq->data[it->current];
}

#endif // STB_deque

#ifndef _MACROLIB_H_
#define _MACROLIB_H_

/*-----------------------------------------------------------------------------
Public Macro */
/* 公共宏 */

#define BITS(type,raw,start,width) ((struct{type s:start;type _bits:width;}*)&raw)->_bits
#define SETBITS(type,raw,start,width,value) do{((struct{type s:start;type _bits:width;}*)&raw)->_bits = value;}while(0)
#define SETBit(n) /*|=*/(0x1<<n)
#define RESETBit(n) /*&=*/(~(0x1<<n))
#define READnBit(value,x,n) /*|=*/((value>>x)&(~(0xFFFFFFFFFFFFFFFF<<n)))
#define SETBitX(n,x) /*|=*/(x<<n)
#define RESETBitX(n, xn)                                                       \
  ({                                                                           \
    unsigned int mask = 0;                                                     \
    unsigned int i = xn;                                                       \
    while (i--) {                                                              \
      mask |= (1 << i);                                                        \
    }                                                                          \
    (~(mask << n));                                                            \
  })

#define arrycp(type, a1, a2)                                                   \
  do {                                                                         \
    typedef struct arry {                                                      \
      type data[sizeof(a1)];                                                   \
    } * arry_p;                                                                \
    typedef struct arry {                                                      \
      type data[sizeof(a2)];                                                   \
    } * arry_p2;                                                               \
    if (sizeof(a1) < sizeof(a2))                                               \
      (*(arry_p)a1) = (*(arry_p)a2);                                           \
    else                                                                       \
      (*(arry_p2)a1) = (*(arry_p2)a2);                                         \
  } while (0)

#define arrycpn(type, a1, a2, size)                                            \
  do {                                                                         \
    typedef struct {                                                           \
      type data[size];                                                         \
    } * arry_p;                                                                \
    *((arry_p)a1) = *((arry_p)a2);                                             \
  } while (0)

#define arrycat(type, a1, ...)                                                 \
  do {                                                                         \
    type arry[] = __VA_ARGS__;                                                 \
    arrycpn(type, a1, arry, sizeof(arry));                                     \
  } while (0)


#define STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(!!(COND))*2-1]
#define STATIC_ASSERT_UNUSED(MSG) static_assertion_##MSG unused##MSG;(void)unused##MSG;

#define EndianTurn(src, type) (src>>sizeof(type)*4|src<<sizeof(type)*4)

#define fn(fn_type,st) (*(fn_type*)&st)(&st)
#define new(fn_type,st) (*(fn_type*)&st)(&st)
#define fnx(fn_type,st,fn_enum) ((fn_type*)(fn_type**)&st)[fn_enum](&st)

#define container_of(ptr, type, member)   ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))
/* write in .c file.
enum fn_id
{
  fn_0,
  fn_1,
  fn_2,
  fn_3,
  fn_4,
  fn_5,
  fn_6,
  fn_7,
  fn_num
};

typedef struct fn_t* fn_p;
typedef void (*fn_f)(fn_p);
typedef struct fn_t{
  fn_p fn_self;
  float   mem1;
  int   mem2;
  char  mem3;
};
typedef struct fnx_t{
  fn_p fn_list[fn_num];
  float mem1;
  int   mem2;
  char  mem3;
};

void fn_1name(fn_p this)
{
  this->mem1 = 123.0;
  this->mem2 = 123;
  this->mem3 = 'F';
}
...

*/

#define arrydef_(type,size) typedef struct{type data[size];}*arry##p
// size 支持任意立即数字和 sizeof 语句  // size supports any immediate number and sizeof expression
#define arrycopy(type,size,a1,a2) do{arrydef_(type,size); (*(arry##p)a1) = (*(arry##p)a2);}while(0)

#endif

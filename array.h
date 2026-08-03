/**The configuration item
 * 
 * */

/**The Usage
 * #include "array.h"
 * */
#ifndef _ARRY_H_
#define _ARRY_H_

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
typedef unsigned long   uint64;

/**
 * Static array copy method
 * - dest: destination array for copy
 * - src : source array to copy.
 * - size: size to copy (must be a numeric literal or sizeof() result)
*/
#define arrycp_u8(dest, src, size)                                             \
do {                                                                           \
  typedef struct {                                                             \
    uint8 _[size];                                                             \
  } * array_p;                                                                 \
  *((array_p)dest) = *((array_p)src);                                          \
} while (0)

#define arrycp_u16(dest, src, size)                                            \
  do {                                                                         \
    typedef struct {                                                           \
      uint16 _[size];                                                          \
    } * array_p;                                                               \
    *((array_p)dest) = *((array_p)src);                                        \
  } while (0)

#define arrycp_u32(dest, src, size)                                            \
  do {                                                                         \
    typedef struct {                                                           \
      uint32 _[size];                                                          \
    } * array_p;                                                               \
    *((array_p)dest) = *((array_p)src);                                        \
  } while (0)

#define arrycp_u64(dest, src, size)                                            \
  do {                                                                         \
    typedef struct {                                                           \
      uint64 _[size];                                                          \
    } * array_p;                                                               \
    *((array_p)dest) = *((array_p)src);                                        \
  } while (0)


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

/**
 * Gets the size of array members of different types.
 * - array destination array.
*/
#define asizeof(array) (sizeof(array) / sizeof(array[0]))

#endif /* _ARRY_H_ */

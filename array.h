/**The configuration item
 * 
 * */

/**The Usage
 * #include "array.h"
 * */
#ifndef _ARRY_H_
#define _ARRY_H_

/**
 * Static array copy method
 * - dest: destination array for copy
 * - src : source array to copy.
 * - size: size to copy (must be a numeric literal or sizeof() result)
*/
#define _static_arrycopy(dest, src, size)
  do {                                                                         \
    typedef struct {                                                           \
      uint16_t _[size];                                                        \
    } * array_p;                                                               \
    *((array_p)dest) = *((array_p)src);                                        \
  } while (0)

/**
 * Gets the size of array members of different types.
 * - array destination array.
*/
#define asizeof(array) (sizeof(array) / sizeof(array[0]))

#endif /* _ARRY_H_ */

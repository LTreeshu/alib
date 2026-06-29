/**The configuration item
 * 
 * */
/**The Usage
 * #include "atype.h"
 * */

#ifndef _ATYPE_H_
#define _ATYPE_H_

/* Usage: typedef TYPE(name_t) {...};
          typedef Type(name_t) {...};
          typedef type(name_t) {...};
          typedef _t(name_t) {...};
 */

#define TYPE(name) struct name name; struct name
#define Type(name) struct name name; struct name
#define type(name) struct name name; struct name

#ifndef _t
#define _t(name) struct name name; struct name
#endif

#endif /* ATYPE_H */

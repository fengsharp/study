#ifndef DS_TYPEDEF_H_
#define DS_TYPEDEF_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
    #define DECLS_BEGIN extern "C" {
    #define DECLS_END   }
#else
    #define DECLS_BEGIN
    #define DECLS_END
#endif

#define SAFE_FREE(p) if(p != NULL) {free(p); p = NULL;}

#define RETURN_IF_FAIL(p) if(!(p)) \
	{printf("%s:%d Warning: "#p" failed.\n", \
		__func__, __LINE__); return;}

#define RETURN_VAL_IF_FAIL(p, ret) if(!(p)) \
	{printf("%s:%d Warning: "#p" failed.\n",\
	__func__, __LINE__); return (ret);}

typedef enum _Ret {
    RET_OK                   = 0,

    RET_INVALID_PARAMS       = 1,
    RET_ERR_ALLOC            = 2,
    RET_ERR_INVALID_CAPACITY = 3,
    RET_ERR_INVALID_RANGE    = 4,
    RET_ERR_MAX_CAPACITY     = 5,
    RET_ERR_KEY_NOT_FOUND    = 6,
    RET_ERR_VALUE_NOT_FOUND  = 7,
    RET_ERR_OUT_OF_RANGE     = 8,

    RET_ITER_END             = 9,
} Ret;

#define MAX_ELEMENTS ((size_t) - 2)

#endif
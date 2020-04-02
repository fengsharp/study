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

/*
#define RETURN_IF_FAIL(cond) if(!(cond)) return
#define RETURN_IF_ASSERT_FAIL(cond) assert(cond);  if(!(cond)) return

#define RETURN_VALUE_IF_FAIL(cond,value) if(!(cond)) return value
#define RETURN_VALUE_IF_ASSERT_FAIL(cond,value) assert(cond);  if(!(cond)) return value
*/

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

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))





#include <unistd.h>
#include <errno.h>

/*
1. r O_RDONLY 
2. r+ O_RDWR
3. w O_WRONLY | O_TRUNC | O_CREAT
4. W+  O_RDWR | O_TRUNC | O_CREAT
*/

ssize_t writen(int fd, const void* pBuf, size_t n)
{
    size_t nLeft = n;
    const char* pData = (const char*)(pBuf);

    while (nLeft > 0) {
        size_t nwrite = write(fd, pData, nLeft);
        if (nwrite <= 0) {
            if (nwrite < 0 && errno == EINTR)
                nwrite = 0;
            else
                return -1;
        } else {
            nLeft -= nwrite;
            pData += nwrite;
        }
    }
    return n;
}

ssize_t readn(int fd, void* pBuf, size_t bufSize) {
    size_t nleft = bufSize;
    char* pData = (char*)(pBuf);

    while (nleft > 0) {
        size_t nread = read(fd, pData, nleft);
        if (nread < 0) {
            if (errno == EINTR || errno == EAGAIN) {
                nread = 0;
            } else {
                return -1;
            }
        } else if (nread == 0) {
            break;
        }

        nleft -= nread;
        pData += nread;
    }

    return bufSize - nleft;
}


#endif
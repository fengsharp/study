#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define SAFE_FREE(p) do { if(p) {free (p); (p) = NULL;}} while(0);

#define RETURN_IF_FAIL(cond) if(!(cond)) return
#define RETURN_IF_ASSERT_FAIL(cond) assert(cond);  if(!(cond)) return

#define RETURN_VALUE_IF_FAIL(cond,value) if(!(cond)) return value
#define RETURN_VALUE_IF_ASSERT_FAIL(cond,value) assert(cond);  if(!(cond)) return value




#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#endif
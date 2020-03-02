#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <assert.h>

/*
    random long int
    long int random(void); [0, RAND_MAX]
    RAND_MAX 不同平台定义不同，centos INT_MAX
*/

static inline void set_rand_seed() {
    srand((unsigned)time(NULL));
}

// ret [0, max]
static inline int random_int(int max) {
    return rand() % max;
}

// ret [min, max]
static inline int random_range(int min, int max) {
    if (min >= max) {
        return max;
    }
    
    return random_int(max - min) + min; // [0, max-min] + min -> [min, max]
}

int main()
{
    srand(10L);

    printf("rand() = %d\n", rand());
    // C99 规定，如果 % 左边的操作数是正数，则模除的结果为正数或零
    printf("random_int(-10) = %d\n", random_int(-10));
    printf("random_int(10) = %d\n", random_int(10));
    printf("random_range(1, 1) = %d\n", random_range(1, 1));
    printf("random_range(3, 1) = %d\n", random_range(1, 1));
    printf("random_range(1, 100) = %d\n", random_range(1, 100));

    return 0;
}

/* console print result: 
    rand() = 1215069295
    random_int(-10) = 8
    random_int(10) = 8
    random_range(1, 1) = 1
    random_range(3, 1) = 1
    random_range(1, 100) = 81
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

static const char* data = "it is used to count words. is it easy?";

#define IS_WORD_CHAR(c)	(isalpha(c) || isdigit(c))
#define IS_END(c) ((c) == '\0')

int words_count(const char* data) {
    enum _State {
        STAT_INIT,
        STAT_IN_WORD,
        STAT_OUT_WORD,
        STAT_END
    } s = STAT_INIT;

    const char* pCursor = data;
    int ret = 0;

    while (s != STAT_END) {
        switch (s) {
            case STAT_INIT:
            {
                if (IS_WORD_CHAR(*pCursor)) {
                    s = STAT_IN_WORD;
                    ++ret;
                } else if (IS_END(*pCursor)) {
                    s = STAT_END;
                } else {
                    s = STAT_OUT_WORD;
                }
            }
            break;
            case STAT_IN_WORD:
            {
                if (IS_END(*pCursor)) {
                    s = STAT_END;
                } else if (!IS_WORD_CHAR(*pCursor)) {
                    s = STAT_OUT_WORD;
                }
            }
            break;
            case STAT_OUT_WORD:
            {
                if (IS_END(*pCursor)) {
                    s = STAT_END;
                } else if (IS_WORD_CHAR(*pCursor)) {
                    s = STAT_IN_WORD;
                    ++ret;
                }
            }
            break;
            default:
            {
                s = STAT_END;
            }
            break;
        }
        ++pCursor;
    }
    return ret;
}

int main()
{
    printf("count:%d\n", words_count(data));
    return 0;
}
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "dlist.h"

static DListRet print_int(void* data)
{
	printf("%d ", (int)data);

	return DLIST_RET_OK;
}


void sumValue(void* ctx, void* data) {
	int* pSum = (int*)ctx;
	int pVal = (int)data;
	*pSum += pVal;
}

typedef struct _MaxCtx {
	bool isFirst;
	int maxVal;
} MaxCtx;

void maxValue(void* ctx, void* data) {
	MaxCtx* pMax = (MaxCtx*)ctx;
	int pVal = (int)data;
	if (pMax->isFirst) {
		pMax->maxVal = pVal;
	} else {
		if (pMax->maxVal < pVal) {
			pMax->maxVal = pVal;
		}
		pMax->isFirst = false;
	}
}

int main(int argc, char* argv[])
{
	int i = 0;
	int n = 10;
	DList* dlist = dlist_create();

	for(i = 0; i < n; i++)
	{
		assert(dlist_append(dlist, (void*)i) == DLIST_RET_OK);
	}
	for(i = 0; i < n; i++)
	{
		assert(dlist_prepend(dlist, (void*)i) == DLIST_RET_OK);
	}

	dlist_print(dlist, print_int);
	puts("");

	int sum = 0;
	dlist_foreach(dlist, sumValue, &sum);
	printf("total sum: %d\n", sum);

	MaxCtx maxCtx = {.isFirst = true, .maxVal = 0};
	dlist_foreach(dlist, maxValue, &maxCtx);
	printf("the max value:%d\n", maxCtx.maxVal);

	dlist_destroy(dlist);

	return 0;
}


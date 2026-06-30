#include "base/base_include.h"
#include "base/base_string.h"

int main(void)
{
	Arena* arena = ArenaAlloc();

	char* a = "ab";

	printf("%lu", Cstring8Length((U8*)a));
}

#include "base/base_include.c"

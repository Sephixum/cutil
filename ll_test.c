#include "base/base_include.h"

int main(void)
{
	Arena* arena = ArenaAlloc();

	String8List list = {0};
	for
		EachIndex(i, 100)
		{
			Str8ListPush(arena, &list, Str8FromF64(arena, (F64)i / 2.0));
		}

	for
		EachNode(it, String8Node, list.first)
		{
			printf("%*s \n", Str8Varg(it->string));
		}
}

#include "base/base_include.c"

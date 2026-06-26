#include "base/base_arena.h"
#include "base/base_hashmap.h"
#include "base/base_include.h"
#include "base/base_include.c"
#include "base/base_string.h"

int main(void)
{
	{
		ThreadContext* main_thread = ThreadContextAlloc();
		ThreadContextSetName(main_thread, Str8Lit("MainThread"));
		ThreadContextSelect(main_thread);
	}

	printf("[%*s] Creating maps ...\n", Str8Varg(ThreadContextSelectedName()));
	{
		Temp	scratch		= ScratchBegin(0, 0);
		String8 thread_name = ThreadContextSelectedName();

		int* values = ArenaPushArray(scratch.arena, int, 100);
		for
			EachIndex(i, 100)
			{
				values[i] = i;
			}

		Str8PtrMap map = Str8PtrMapAlloc(scratch.arena, 256);
		for
			EachIndex(i, 100)
			{
				Str8PtrMapInsert(&map, Str8FromU64(scratch.arena, i, 10, 3, 0), values + i);
			}

		String8Array keys = Str8PtrMapKeys(scratch.arena, &map);
		for
			EachIndex(i, map.count)
			{
				fprintf(stdout, "%*s \n", Str8Varg(keys.v[i]));
			}

		int* found = Str8PtrMapLookup(&map, Str8Lit("042"));
		if (found)
		{
			printf("found: %d\n", (*found));
		}

		int** vals = (int**)Str8PtrMapValues(scratch.arena, &map);
		for
			EachIndex(i, map.count)
			{
				fprintf(stdout, "%d \n", *vals[i]);
			}
	}
}

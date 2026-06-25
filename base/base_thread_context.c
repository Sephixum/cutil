#include "base_thread_context.h"
#include "base_arena.h"
#include "base_typedefs.h"
#include <threads.h>

C_LINKAGE thread_local ThreadContext* thread_context_thread_local;

internal ThreadContext* ThreadContextAlloc(void)
{
	Arena*		   arena_0 = ArenaAlloc();
	Arena*		   arena_1 = ArenaAlloc();
	ThreadContext* tctx	   = ArenaPushArray(arena_0, ThreadContext, 1);
	tctx->scratch_pool[0]  = arena_0;
	tctx->scratch_pool[1]  = arena_1;
	return tctx;
}

internal void ThreadContextRelease(ThreadContext* tctx)
{
	ArenaRelease(tctx->scratch_pool[1]);
	ArenaRelease(tctx->scratch_pool[0]);
}
internal void ThreadContextSelect(ThreadContext* tctx)
{
	thread_context_thread_local = tctx;
}

internal ThreadContext* ThreadContextSelected(void)
{
	return thread_context_thread_local;
}

internal Arena* ThreadContextGetArenaScratch(Arena** conflicts, U64 count)
{
	ThreadContext* tctx		 = ThreadContextSelected();
	Arena*		   result	 = 0;
	Arena**		   arena_ptr = tctx->scratch_pool;

	for (U64 i = 0; i < ArrayCount(tctx->scratch_pool); i += 1, arena_ptr += 1)
	{
		Arena** conflict_ptr = conflicts;
		B32		has_conflict = 0;
		for (U64 j = 0; j < count; j += 1, conflict_ptr += 1)
		{
			if (*arena_ptr == *conflict_ptr)
			{
				has_conflict = 1;
				break;
			}
		}
		if (!has_conflict)
		{
			result = *arena_ptr;
			break;
		}
	}

	return result;
}

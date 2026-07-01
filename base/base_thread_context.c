#include "base_thread_context.h"
#include "base_arena.h"
#include "base_core.h"
#include "base_typedefs.h"
#include <threads.h>

C_LINKAGE thread_local ThreadContext* tctx_thread_local;

internal ThreadContext* ThreadContext_Make(void)
{
	Arena*		   arena_0 = Arena_Make();
	Arena*		   arena_1 = Arena_Make();
	ThreadContext* tctx	   = Arena_PushArray(arena_0, ThreadContext, 1);
	tctx->scratch_pool[0]  = arena_0;
	tctx->scratch_pool[1]  = arena_1;
	return tctx;
}

internal void ThreadContextRelease(ThreadContext* tctx)
{
	Arena_Destroy(tctx->scratch_pool[1]);
	Arena_Destroy(tctx->scratch_pool[0]);
}
internal void ThreadContext_Select(ThreadContext* tctx)
{
	tctx_thread_local = tctx;
}

internal void ThreadContext_SetName(ThreadContext* tctx, String8 name)
{
	U64 copy_size = name.size < 32 ? name.size : 31;
	MemoryCopy(tctx->thread_name, name.str, copy_size);
	tctx->thread_name_size = copy_size;
}

internal String8 ThreadContext_GetName(ThreadContext* tctx)
{
	return String8_Make(tctx->thread_name, tctx->thread_name_size);
}

internal ThreadContext* ThreadContext_Selected(void)
{
	return tctx_thread_local;
}

internal String8 ThreadContext_SelectedName(void)
{
	return ThreadContext_GetName(ThreadContext_Selected());
}

internal Arena* ThreadContext_GetArenaScratch(Arena** conflicts, U64 count)
{
	ThreadContext* tctx		 = ThreadContext_Selected();
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

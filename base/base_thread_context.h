#ifndef BASE_THREAD_CONTEXT_H
#define BASE_THREAD_CONTEXT_H

#include "base_arena.h"
#include "base_string.h"
#include "base_typedefs.h"

typedef struct ThreadContext ThreadContext;
struct ThreadContext
{
	Arena* scratch_pool[2];

	U8	thread_name[32];
	U64 thread_name_size;
};

internal ThreadContext* ThreadContext_Make(void);
internal void			ThreadContext_Destroy(ThreadContext* tctx);
internal void			ThreadContext_Select(ThreadContext* tctx);
internal void			ThreadContext_SetName(ThreadContext* tctx, String8 name);
internal String8		ThreadContext_GetName(ThreadContext* tctx);
internal ThreadContext* ThreadContext_Selected(void);
internal String8		ThreadContext_SelectedName(void);

internal Arena* ThreadContext_GetArenaScratch(Arena** conflicts, U64 count);
#define Scratch_Begin(conflicts, count) Arena_TempBegin(ThreadContextGetArenaScratch((conflicts), (count)))
#define Scratch_End(scratch) Arena_TempEnd(scratch)

#endif // BASE_THREAD_CONTEXT_H

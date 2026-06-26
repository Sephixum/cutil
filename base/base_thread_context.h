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

internal ThreadContext* ThreadContextAlloc(void);
internal void			ThreadContextRelease(ThreadContext* tctx);
internal void			ThreadContextSelect(ThreadContext* tctx);
internal void			ThreadContextSetName(ThreadContext* tctx, String8 name);
internal String8		ThreadContextGetName(ThreadContext* tctx);
internal ThreadContext* ThreadContextSelected(void);
internal String8		ThreadContextSelectedName(void);

internal Arena* ThreadContextGetArenaScratch(Arena** conflicts, U64 count);
#define ScratchBegin(conflicts, count) ArenaTempBegin(ThreadContextGetArenaScratch((conflicts), (count)))
#define ScratchEnd(scratch) ArenaTempEnd(scratch)

#endif // BASE_THREAD_CONTEXT_H

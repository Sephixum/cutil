#include "base_arena.h"
#include "base_core.h"
#include "base_typedefs.h"
#include "base_system.h"
#include "base_memory.h"

internal Arena* Arena_Make_(ArenaParams* params)
{
	U64 reserve_size = params->reserve_size;
	U64 commit_size	 = params->commit_size;

	void* base = params->optional_backing_buffer;
	if (base == 0)
	{
		// Round up the reserve/commit sizes
		if (params->flags & ArenaFlag_LargePages)
		{
			reserve_size = AlignPow2(reserve_size, GetSystemInfo()->large_page_size);
			commit_size	 = AlignPow2(commit_size, GetSystemInfo()->large_page_size);
		}
		else
		{
			reserve_size = AlignPow2(reserve_size, GetSystemInfo()->page_size);
			commit_size	 = AlignPow2(commit_size, GetSystemInfo()->page_size);
		}

		if (params->flags & ArenaFlag_LargePages)
		{
			base = MemoryReserveLarge(reserve_size);
			MemoryCommitLarge(base, commit_size);
		}
		else
		{
			base = MemoryReserve(reserve_size);
			MemoryCommit(base, commit_size);
		}
	}

	Arena* arena				= base;
	arena->current				= arena;
	arena->flags				= params->flags;
	arena->commit_size			= params->commit_size;
	arena->reserve_size			= params->reserve_size;
	arena->base_position		= 0;
	arena->position				= ARENA_HEADER_SIZE;
	arena->commit				= commit_size;
	arena->reserve				= reserve_size;
	arena->allocation_site_file = params->allocation_site_file;
	arena->allocation_site_line = params->allocation_site_line;
	arena->name					= params->name;
#if ARENA_FREE_LIST
	arena->free_last = 0;
#endif // ARENA_FREE_LIST

	return arena;
}

internal void Arena_Destroy(Arena* arena)
{
	for (Arena *n = arena->current, *prev = 0; n != 0; n = prev)
	{
		prev = n->prev;
		MemoryRelease(n, n->reserve);
	}
}

internal void* Arena_Push(Arena* arena, U64 size, U64 align, B32 zero)
{
	Arena* current = arena->current;
	U64	   pos_pre = AlignPow2(current->position, align);
	U64	   pos_pst = pos_pre + size;

	U64 size_to_zero = 0;
	if (zero)
	{
		size_to_zero = Min(current->commit, pos_pst) - pos_pre;
	}

	// Chain If Needed
	if (current->reserve < pos_pst && !(arena->flags & ArenaFlag_NoChain))
	{
		Arena* new_block = 0;
#if ARENA_FREE_LIST
		{
			Arena* prev_block;
			for (new_block = arena->free_last, prev_block = 0; new_block != 0;
				 prev_block = new_block, new_block = new_block->prev)
			{
				if (new_block->reserve >= AlignPow2(new_block->position, align) + size)
				{
					if (prev_block != 0)
					{
						prev_block->prev = new_block->prev;
					}
					else
					{
						arena->free_last = new_block->prev;
					}
					break;
				}
			}
		}
#endif // ARENA_FREE_LIST

		if (new_block == 0)
		{
			U64 reserve_size = current->reserve_size;
			U64 commit_size	 = current->commit_size;
			if (size + ARENA_HEADER_SIZE > reserve_size)
			{
				reserve_size = AlignPow2(size + ARENA_HEADER_SIZE, align);
				commit_size	 = AlignPow2(size + ARENA_HEADER_SIZE, align);
			}
			new_block = Arena_Alloc(.reserve_size		  = reserve_size,
									.commit_size		  = commit_size,
									.flags				  = current->flags,
									.allocation_site_file = current->allocation_site_file,
									.allocation_site_line = current->allocation_site_line);

			size_to_zero = 0;
		}
		else
		{
			size_to_zero = size;
		}

		new_block->base_position = current->base_position + current->reserve;
		SLLStackPush_N(arena->current, new_block, prev);

		current = new_block;
		pos_pre = AlignPow2(current->position, align);
		pos_pst = pos_pre + size;
	}

	// Commit New Pages If Needed
	if (current->commit < pos_pst)
	{
		U64 commit_pst_aligned = pos_pst + current->commit_size - 1;
		commit_pst_aligned -= commit_pst_aligned % current->commit_size;
		U64 commit_pst_clamped = ClampTop(commit_pst_aligned, current->reserve);
		U64 commit_size		   = commit_pst_clamped - current->commit;
		U8* commit_ptr		   = (U8*)current + current->commit;
		if (current->flags & ArenaFlag_LargePages)
		{
			MemoryCommitLarge(commit_ptr, commit_size);
		}
		else
		{
			MemoryCommit(commit_ptr, commit_size);
		}
		current->commit = commit_pst_clamped;
	}

	void* result = 0;
	if (current->commit >= pos_pst)
	{
		result			  = (U8*)current + pos_pre;
		current->position = pos_pst;
		MemoryZero(result, size_to_zero);
	}

	return result;
}

internal U64 Arena_Pos(Arena* arena)
{
	Arena* current = arena->current;
	U64	   pos	   = current->base_position + current->position;
	return pos;
}

internal void Arena_PopTo(Arena* arena, U64 pos)
{
	U64	   big_pos = ClampBot(ARENA_HEADER_SIZE, pos);
	Arena* current = arena->current;

#if ARENA_FREE_LIST
	for (Arena* prev = 0; current->base_position >= big_pos; current = prev)
	{
		prev			  = current->prev;
		current->position = ARENA_HEADER_SIZE;
		SLLStackPush_N(arena->free_last, current, prev);
	}
#else
	for (Arena* prev = 0; current->base_position >= big_pos; current = prev)
	{
		prev = current->prev;
		MemoryRelease(current, current->reserve);
	}
#endif // ARENA_FREE_LIST

	arena->current = current;
	U64 new_pos	   = big_pos - current->base_position;
	assert(new_pos <= current->position);
	current->position = new_pos;
}

internal void Arena_Clear(Arena* arena)
{
	Arena_PopTo(arena, 0);
}

internal void Arena_Pop(Arena* arena, U64 amt)
{
	U64 pos_old = Arena_Pos(arena);
	U64 pos_new = pos_old;
	if (amt < pos_old)
	{
		pos_new = pos_old - amt;
	}
	Arena_PopTo(arena, pos_new);
}

internal Temp Arena_TempBegin(Arena* arena)
{
	U64 pos = Arena_Pos(arena);
	return (Temp){arena, pos};
}

internal void Arena_TempEnd(Temp temp)
{
	Arena_PopTo(temp.arena, temp.pos);
}

global thread_local Arena* scratches[2] = {0};

internal Temp Arena_GetScratch(Arena** conflicts, U64 conflict_count)
{
	for (U64 i = 0; i < ArrayCount(scratches); ++i)
	{
		B32 is_conflicting = 0;
		for (U64 j = 0; j < conflict_count; ++j)
		{
			if (scratches[i] == conflicts[j])
			{
				is_conflicting = 1;
				break;
			}
		}

		if (!is_conflicting)
		{
			return Arena_TempBegin(scratches[i]);
		}
	}

	return (Temp){0};
}

#ifndef ARENA_H
#define ARENA_H

#include "base_core.h"
#include "base_typedefs.h"

#define ARENA_HEADER_SIZE 128
#define ARENA_FREE_LIST 1

typedef U64 ArenaFlags;
enum
{
	ArenaFlag_NoChain	 = Bit(1),
	ArenaFlag_LargePages = Bit(2)
};

typedef struct ArenaParams ArenaParams;
struct ArenaParams
{
	ArenaFlags flags;
	U64		   reserve_size;
	U64		   commit_size;
	void*	   optional_backing_buffer;
	char*	   allocation_site_file;
	int		   allocation_site_line;
	char*	   name;
};

typedef struct Arena Arena;
struct Arena
{
	Arena* prev;
	Arena* current;

	ArenaFlags flags;

	U64 commit_size;
	U64 reserve_size;
	U64 base_position;

	U64 position;
	U64 commit;
	U64 reserve;

	char* allocation_site_file;
	int	  allocation_site_line;
	char* name;

#if ARENA_FREE_LIST
	Arena* free_last;
#endif // ARENA_FREE_LIST
};
StaticAssert(sizeof(Arena) <= ARENA_HEADER_SIZE, Arena_Header_size_is_bigger_than_ARENA_HEADER_SIZE);

typedef struct Temp Temp;
struct Temp
{
	Arena* arena;
	U64	   pos;
};

global U64		  arena_default_reserve_size = MB(64);
global U64		  arena_default_commit_size	 = MB(64);
global ArenaFlags arena_default_flags		 = 0;

internal Arena* Arena_Make_(ArenaParams* params);
#define Arena_Make(...)                                                                                                \
	Arena_Make_(&(ArenaParams){.reserve_size		 = arena_default_reserve_size,                                     \
							   .commit_size			 = arena_default_commit_size,                                      \
							   .flags				 = arena_default_flags,                                            \
							   .allocation_site_file = __FILE__,                                                       \
							   .allocation_site_line = __LINE__,                                                       \
							   __VA_ARGS__})

internal void Arena_Destroy(Arena* arena);

internal void* Arena_Push(Arena* arena, U64 size, U64 align, B32 zero);
internal U64   Arena_Pos(Arena* arena);
internal void  Arena_PopTo(Arena* arena, U64 pos);

internal void Arena_Clear(Arena* arena);
internal void Arena_Pop(Arena* arena, U64 amt);

internal Temp Arena_TempBegin(Arena* arena);
internal void Arena_TempEnd(Temp temp);

#define Arena_PushArrayNoZeroAligned(a, T, c, align) (T*)(Arena_Push((a), sizeof(T) * (c), (align), 0))
#define Arena_PushArrayAligned(a, T, c, align) (T*)(Arena_Push((a), sizeof(T) * (c), (align), 1))
#define Arena_PushArrayNoZero(a, T, c) Arena_PushArrayNoZeroAligned(a, T, c, Max(8, alignof(T)))
#define Arena_PushArray(a, T, c) Arena_PushArrayAligned(a, T, c, Max(8, alignof(T)))

internal Temp Arena_GetScratch(Arena** conflicts, U64 conflict_count);
#define Arena_ReleaseScratch(t) Arena_TempEnd(t)

#endif // ARENA_H

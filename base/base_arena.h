#ifndef ARENA_H
#define ARENA_H

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
static_assert(sizeof(Arena) <= ARENA_HEADER_SIZE, "Arena Header size is bigger than ARENA_HEADER_SIZE !!");

typedef struct Temp Temp;
struct Temp
{
	Arena* arena;
	U64	   pos;
};

global U64		  arena_default_reserve_size = MB(64);
global U64		  arena_default_commit_size	 = MB(64);
global ArenaFlags arena_default_flags		 = 0;

internal Arena* ArenaAlloc_(ArenaParams* params);
#define ArenaAlloc(...)                                                                                                \
	ArenaAlloc_(&(ArenaParams){.reserve_size		 = arena_default_reserve_size,                                     \
							   .commit_size			 = arena_default_commit_size,                                      \
							   .flags				 = arena_default_flags,                                            \
							   .allocation_site_file = __FILE__,                                                       \
							   .allocation_site_line = __LINE__,                                                       \
							   __VA_ARGS__})

internal void ArenaRelease(Arena* arena);

internal void* ArenaPush(Arena* arena, U64 size, U64 align, B32 zero);
internal U64   ArenaPos(Arena* arena);
internal void  ArenaPopTo(Arena* arena, U64 pos);

internal void ArenaClear(Arena* arena);
internal void ArenaPop(Arena* arena, U64 amt);

internal Temp ArenaTempBegin(Arena* arena);
internal void ArenaTempEnd(Temp temp);

#define ArenaPushArrayNoZeroAligned(a, T, c, align) (T*)(ArenaPush((a), sizeof(T) * (c), (align), 0))
#define ArenaPushArrayAligned(a, T, c, align) (T*)(ArenaPush((a), sizeof(T) * (c), (align), 1))
#define ArenaPushArrayNoZero(a, T, c) ArenaPushArrayNoZeroAligned(a, T, c, Max(8, alignof(T)))
#define ArenaPushArray(a, T, c) ArenaPushArrayAligned(a, T, c, Max(8, alignof(T)))

internal Temp ArenaGetScratch(Arena** conflicts, U64 conflict_count);
#define ArenaReleaseScratch(t) ArenaTempEnd(t)

internal void ArenaEnsureScratchAllocation(void);

#endif // ARENA_H

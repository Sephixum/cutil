#ifndef BASE_HASHMAP_H
#define BASE_HASHMAP_H

#include "base_typedefs.h"
#include "base_arena.h"

////////////////////////
// Utility

internal U64 NextPowerOfTwo(U64 x);

////////////////////////
// Map Interface

typedef struct MapSlot MapSlot;
struct MapSlot
{
	B32		 occupied; // Slot holds a live entry
	B32		 deleted;  // Tombstone: skipped during probe, reusable
	void*	 key;
	void*	 value;
	MapSlot* next_free; // Intrusive free list chain (only valid when deleted)
};

//////////////////////////////
// These are the only functions needed
// to be implemented for the type owner

typedef U64 (*map_hash_fn)(void* key);
typedef B32 (*map_equal_fn)(void* lhs, void* rhs);

typedef struct IMap IMap;
struct IMap
{
	MapSlot*	 data;
	MapSlot*	 free_list; // Head of reusable deleted slots
	U64			 key_size;
	U64			 value_size;
	U64			 cap;	// Power of 2
	U64			 count; // Live (non-deleted) slots
	map_hash_fn	 hash;
	map_equal_fn equal;
};

internal IMap  IMapAlloc(Arena* arena, U64 cap, U64 key_size, U64 value_size, map_hash_fn hash, map_equal_fn equal);
internal void  IMapInsert(Arena* arena, IMap* map, void* key, void* value);
internal B32   IMapRemove(IMap* map, void* key);
internal void* IMapLookup(IMap* map, void* key);
internal void  IMapResize(Arena* arena, IMap* map, U64 new_cap);
internal void  IMapClear(IMap* map);

#endif // BASE_HASHMAP_H

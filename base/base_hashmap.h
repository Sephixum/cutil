#ifndef BASE_HASHMAP_H
#define BASE_HASHMAP_H

#include "base_typedefs.h"
#include "base_arena.h"
#include "base_string.h"
#include "base_hash.h"

////////////////////////////////
//
// How to use this file
// --------------------
// There is no single generic HashMap type.
// Instead, define one concrete map type per key/value pair you need.
//
// Step 1: define a slot struct
//
//   typedef struct MySlot MySlot;
//   struct MySlot
//   {
//       U32      key;
//       Entity  *val;
//       B8       occupied;
//   };
//
// Step 2: define the map struct
//
//   typedef struct MyMap MyMap;
//   struct MyMap
//   {
//       MySlot *slots;
//       U64     cap;    // always power of 2
//       U64     count;
//   };
//
// Step 3: implement Alloc / Insert / Lookup using the helpers below.
//
// See the two ready-made examples at the bottom of this file:
//   - Str8PtrMap  (String8 -> void*)
//   - U64PtrMap   (U64     -> void*, also works for U32/U16/U8 keys)
//
////////////////////////////////

////////////////////////////////
// Internal probe helper
//
// Wraps the linear-probe index arithmetic so your Insert/Lookup
// functions don't repeat it. Cap must be a power of 2.
//
#define HashMapSlotIndex(hash, i, cap) (((hash) + (i)) & ((cap) - 1))

////////////////////////////////
//
// Example map 1: String8 -> void*
//
// Usage:
//   Str8PtrMap map = Str8PtrMapMake(arena, 256);
//   Str8PtrMapInsert(&map, Str8Lit("health"), (void*)entity_ptr);
//   void *result = Str8PtrMapLookup(&map, Str8Lit("health"));
//   if(result) { Entity *e = (Entity*)(*result); }
//
////////////////////////////////

typedef struct ByteArray ByteArray;
struct ByteArray
{
	void *v;
	U64	  count;
	U64	  total_size;
};

typedef struct String8PtrSlot String8PtrSlot;
struct String8PtrSlot
{
	String8 key;
	void   *val;
	B8		occupied;
};

typedef struct Str8PtrMap Str8PtrMap;
struct Str8PtrMap
{
	String8PtrSlot *slots;
	U64				cap;
	U64				count;
};

internal Str8PtrMap	  Str8PtrMapAlloc(Arena *arena, U64 cap);
internal void		  Str8PtrMapInsert(Str8PtrMap *map, String8 key, void *val);
internal void		 *Str8PtrMapLookup(Str8PtrMap *map, String8 key);
internal String8Array Str8PtrMapKeysCopy(Arena *arena, Str8PtrMap *map);
internal ByteArray	  Str8PtrMapValuesCopy(Arena *arena, Str8PtrMap *map);

////////////////////////////////
//
// Example map 2: U64 -> void*
//
// Works for any integer key — just cast u8/u16/u32 to U64 at the call site:
//   U64PtrMapInsert(&map, (U64)my_u32_id, ptr);
//
// Usage:
//   U64PtrMap map = U64PtrMapMake(arena, 256);
//   U64PtrMapInsert(&map, entity_id, (void*)entity_ptr);
//   void **result = U64PtrMapLookup(&map, entity_id);
//
////////////////////////////////

typedef struct U64PtrSlot U64PtrSlot;
struct U64PtrSlot
{
	U64	  key;
	void *val;
	B8	  occupied;
};

typedef struct U64PtrMap U64PtrMap;
struct U64PtrMap
{
	U64PtrSlot *slots;
	U64			cap;
	U64			count;
};

typedef struct U64Array U64Array;
struct U64Array
{
	U64 *v;
	U64	 count;
	U64	 total_size;
};

internal U64PtrMap U64PtrMapAlloc(Arena *arena, U64 cap);
internal void	   U64PtrMapInsert(U64PtrMap *map, U64 key, void *val);
internal void	 **U64PtrMapLookup(U64PtrMap *map, U64 key);
internal U64Array  U64PtrMapKeysCopy(Arena *arena, U64PtrMap *map);
internal ByteArray U64PtrMapValuesCopy(Arena *arena, U64PtrMap *map);

#endif // BASE_HASHMAP_H

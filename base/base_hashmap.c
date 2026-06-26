#include "base_hashmap.h"
#include "base_arena.h"
#include "base_string.h"

////////////////////////////////
// Str8PtrMap

internal Str8PtrMap Str8PtrMapAlloc(Arena *arena, U64 cap)
{
	// Cap must be power of 2 — assert in debug, silently round up in release
	Assert((cap & (cap - 1)) == 0 && "Str8PtrMap cap must be a power of 2");
	Str8PtrMap map = {0};
	map.slots	   = ArenaPushArray(arena, String8PtrSlot, cap);
	map.cap		   = cap;
	return map;
}

internal void Str8PtrMapInsert(Str8PtrMap *map, String8 key, void *val)
{
	U64 hash = HashStr8(key);
	for (U64 i = 0; i < map->cap; i++)
	{
		U64				idx	 = HashMapSlotIndex(hash, i, map->cap);
		String8PtrSlot *slot = &map->slots[idx];
		if (!slot->occupied)
		{
			slot->key	   = key;
			slot->val	   = val;
			slot->occupied = 1;
			map->count += 1;
			return;
		}
		if (Str8Match(slot->key, key, (StringMatchFlags)0))
		{
			// Overwrite existing key
			slot->val = val;
			return;
		}
	}
	Assert(!"Str8PtrMap is full");
}

internal void *Str8PtrMapLookup(Str8PtrMap *map, String8 key)
{
	U64 hash = HashStr8(key);
	for (U64 i = 0; i < map->cap; i++)
	{
		U64				idx	 = HashMapSlotIndex(hash, i, map->cap);
		String8PtrSlot *slot = &map->slots[idx];
		if (!slot->occupied)
		{
			return 0;
		}
		if (Str8Match(slot->key, key, (StringMatchFlags)0))
		{
			return slot->val;
		}
	}
	return 0;
}

internal String8Array Str8PtrMapKeysCopy(Arena *arena, Str8PtrMap *map)
{
	String8Array result = {.v		   = ArenaPushArray(arena, String8, map->count),
						   .total_size = map->count * sizeof(String8),
						   .count	   = map->count};

	U64 n = 0;
	for
		EachIndex(i, map->cap)
		{
			if (map->slots[i].occupied)
			{
				result.v[n++] = map->slots[i].key;
			}
		}

	return result;
}

internal ByteArray Str8PtrMapValuesCopy(Arena *arena, Str8PtrMap *map)
{
	ByteArray result = {
		.v			= ArenaPushArray(arena, void *, map->count),
		.count		= map->count,
		.total_size = map->count * sizeof(void *),
	};

	void **slots = (void **)result.v;
	U64	   n	 = 0;
	for
		EachIndex(i, map->cap)
		{
			if (map->slots[i].occupied)
			{
				slots[n++] = map->slots[i].val;
			}
		}
	return result;
}

////////////////////////////////
// U64PtrMap

internal U64PtrMap U64PtrMapAlloc(Arena *arena, U64 cap)
{
	Assert((cap & (cap - 1)) == 0 && "U64PtrMap cap must be a power of 2");
	U64PtrMap map = {0};
	map.slots	  = ArenaPushArray(arena, U64PtrSlot, cap);
	map.cap		  = cap;
	return map;
}

internal void U64PtrMapInsert(U64PtrMap *map, U64 key, void *val)
{
	U64 hash = HashU64(key);
	for (U64 i = 0; i < map->cap; i++)
	{
		U64			idx	 = HashMapSlotIndex(hash, i, map->cap);
		U64PtrSlot *slot = &map->slots[idx];
		if (!slot->occupied)
		{
			slot->key	   = key;
			slot->val	   = val;
			slot->occupied = 1;
			map->count += 1;
			return;
		}
		if (slot->key == key)
		{
			slot->val = val;
			return;
		}
	}
	Assert(!"U64PtrMap is full");
}

internal void **U64PtrMapLookup(U64PtrMap *map, U64 key)
{
	U64 hash = HashU64(key);
	for (U64 i = 0; i < map->cap; i++)
	{
		U64			idx	 = HashMapSlotIndex(hash, i, map->cap);
		U64PtrSlot *slot = &map->slots[idx];
		if (!slot->occupied)
		{
			return 0;
		}
		if (slot->key == key)
		{
			return &slot->val;
		}
	}
	return 0;
}

internal U64Array U64PtrMapKeysCopy(Arena *arena, U64PtrMap *map)
{
	U64Array result = {.v		   = ArenaPushArray(arena, U64, map->count),
					   .total_size = map->count * sizeof(U64),
					   .count	   = map->count};

	U64 n = 0;
	for
		EachIndex(i, map->cap)
		{
			if (map->slots[i].occupied)
			{
				result.v[n++] = map->slots[i].key;
			}
		}

	return result;
}

internal ByteArray U64PtrMapValuesCopy(Arena *arena, U64PtrMap *map)
{
	ByteArray result = {
		.v			= ArenaPushArray(arena, void *, map->count),
		.count		= map->count,
		.total_size = map->count * sizeof(void *),
	};

	void **slots = (void **)result.v;
	U64	   n	 = 0;
	for
		EachIndex(i, map->cap)
		{
			if (map->slots[i].occupied)
			{
				slots[n++] = map->slots[i].val;
			}
		}
	return result;
}

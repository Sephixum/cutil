#include "base_hashmap.h"
#include "base_arena.h"
#include "base_core.h"

////////////////////////
// Utility

internal U64 NextPowerOfTwo(U64 x)
{
	if (x == 0)
		return 1;
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x |= x >> 32;
	x++;
	return x;
}

////////////////////////
// Internal helpers

// Pop a slot from the free list
internal MapSlot* MapFreeListPop(IMap* map)
{
	if (!map->free_list)
		return 0;
	MapSlot* slot	= map->free_list;
	map->free_list	= slot->next_free;
	slot->next_free = 0;
	return slot;
}

// Push a slot onto the free list
internal void MapFreeListPush(IMap* map, MapSlot* slot)
{
	slot->next_free = map->free_list;
	map->free_list	= slot;
}

// Write key+value into a slot, marking it live
// key and value must already be arena-allocated
internal void MapSlotWrite(MapSlot* slot, void* key, void* value)
{
	slot->key		= key;
	slot->value		= value;
	slot->occupied	= 1;
	slot->deleted	= 0;
	slot->next_free = 0;
}

// Find the probe index for a key, or return -1 if not found
// Returns the slot pointer directly for convenience
internal MapSlot* MapProbe(IMap* map, void* key)
{
	U64 hash = map->hash(key);
	U64 idx	 = hash & (map->cap - 1);
	for (U64 step = 0; step < map->cap; ++step)
	{
		U64		 slot_idx = (idx + step) & (map->cap - 1);
		MapSlot* slot	  = &map->data[slot_idx];
		if (slot->deleted)
		{
			continue; // tombstone, keep probing
		}
		if (!slot->occupied)
		{
			return 0; // empty, key not present
		}
		if (map->equal(slot->key, key))
		{
			return slot;
		}
	}
	return 0;
}

// Re-slot an already-allocated entry into the current table (used by resize)
internal void MapReSlot(IMap* map, void* key, void* value)
{
	U64 hash = map->hash(key);
	U64 idx	 = hash & (map->cap - 1);
	for (U64 step = 0; step < map->cap; ++step)
	{
		U64		 slot_idx = (idx + step) & (map->cap - 1);
		MapSlot* slot	  = &map->data[slot_idx];
		// During resize the new table has no tombstones, so deleted won't appear
		if (!slot->occupied)
		{
			MapSlotWrite(slot, key, value);
			map->count++;
			return;
		}
	}
	Assert(!"Map full during rehash!");
}

////////////////////////
// Alloc

internal IMap MapAlloc(Arena* arena, U64 cap, U64 key_size, U64 value_size, map_hash_fn hash, map_equal_fn equal)
{
	cap			   = NextPowerOfTwo(cap);
	MapSlot* slots = ArenaPushArray(arena, MapSlot, cap);
	MemoryZero(slots, sizeof(MapSlot) * cap);
	return (IMap){
		.data		= slots,
		.free_list	= 0,
		.cap		= cap,
		.count		= 0,
		.key_size	= key_size,
		.value_size = value_size,
		.hash		= hash,
		.equal		= equal,
	};
}

////////////////////////
// Insert / Update

internal void MapInsert(Arena* arena, IMap* map, void* key, void* value)
{
	// Resize at 75% load
	if (map->count * 4 > map->cap * 3)
	{
		MapResize(arena, map, map->cap * 2);
	}

	// Check if key already exists — update in place
	MapSlot* existing = MapProbe(map, key);
	if (existing)
	{
		U8* new_value = ArenaPushArray(arena, U8, map->value_size);
		MemoryCopy(new_value, value, map->value_size);
		existing->value = new_value;
		return;
	}

	// Allocate key+value storage
	U8* key_storage	  = ArenaPushArray(arena, U8, map->key_size);
	U8* value_storage = ArenaPushArray(arena, U8, map->value_size);
	MemoryCopy(key_storage, key, map->key_size);
	MemoryCopy(value_storage, value, map->value_size);

	// Try to reuse a free list slot first
	MapSlot* reused = MapFreeListPop(map);
	if (reused)
	{
		MapSlotWrite(reused, key_storage, value_storage);
		map->count++;
		return;
	}

	// Otherwise probe for an empty slot
	U64 hash = map->hash(key);
	U64 idx	 = hash & (map->cap - 1);
	for (U64 step = 0; step < map->cap; ++step)
	{
		U64		 slot_idx = (idx + step) & (map->cap - 1);
		MapSlot* slot	  = &map->data[slot_idx];
		if (!slot->occupied || slot->deleted)
		{
			MapSlotWrite(slot, key_storage, value_storage);
			map->count++;
			return;
		}
	}

	Assert(!"Map full!");
}

////////////////////////
// Lookup

internal void* MapLookup(IMap* map, void* key)
{
	MapSlot* slot = MapProbe(map, key);
	return slot ? slot->value : 0;
}

////////////////////////
// Remove

internal B32 MapRemove(IMap* map, void* key)
{
	U64 hash = map->hash(key);
	U64 idx	 = hash & (map->cap - 1);
	for (U64 step = 0; step < map->cap; ++step)
	{
		U64		 slot_idx = (idx + step) & (map->cap - 1);
		MapSlot* slot	  = &map->data[slot_idx];
		if (slot->deleted)
		{
			continue;
		}

		if (!slot->occupied)
		{
			return 0;
		}

		if (map->equal(slot->key, key))
		{
			slot->occupied = 0;
			slot->deleted  = 1;
			map->count--;
			MapFreeListPush(map, slot); // Push onto free list so this slot can be reused
			return 1;
		}
	}
	return 0;
}

////////////////////////
// Resize

internal void MapResize(Arena* arena, IMap* map, U64 new_cap)
{
	new_cap = NextPowerOfTwo(new_cap);
	if (new_cap <= map->cap)
		return;

	MapSlot* old_slots = map->data;
	U64		 old_cap   = map->cap;

	MapSlot* new_slots = ArenaPushArray(arena, MapSlot, new_cap);
	MemoryZero(new_slots, sizeof(MapSlot) * new_cap);

	map->data	   = new_slots;
	map->cap	   = new_cap;
	map->count	   = 0;
	map->free_list = 0; // deleted slots are gone, fresh table

	for (U64 i = 0; i < old_cap; ++i)
	{
		MapSlot* old_slot = &old_slots[i];
		if (old_slot->occupied && !old_slot->deleted)
		{
			// Reuse existing arena pointers, no extra copies
			MapReSlot(map, old_slot->key, old_slot->value);
		}
	}
}

////////////////////////
// Clear

internal void MapClear(IMap* map)
{
	MemoryZero(map->data, sizeof(MapSlot) * map->cap);
	map->free_list = 0;
	map->count	   = 0;
}

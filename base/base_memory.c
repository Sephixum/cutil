#include "base_typedefs.h"

internal B32 MemoryIsZero(void *ptr, U64 size)
{
	B32 result = 1;

	// Break down size
	U64 extra  = (size & 0x7);
	U64 count8 = (size >> 3);

	// Check with 8-byte stride
	U64 *p64 = (U64 *)ptr;
	if (result)
	{
		for (U64 i = 0; i < count8; i += 1, p64 += 1)
		{
			if (*p64 != 0)
			{
				result = 0;
				goto done;
			}
		}
	}

	// Check extra
	if (result)
	{
		U8 *p8 = (U8 *)p64;
		for (U64 i = 0; i < extra; i += 1, p8 += 1)
		{
			if (*p8 != 0)
			{
				result = 0;
				goto done;
			}
		}
	}

done:;
	return result;
}

internal void MemoryWrite16(void *ptr, U16 v)
{
	MemoryCopy(ptr, &v, sizeof(v));
}

internal void MemoryWrite32(void *ptr, U32 v)
{
	MemoryCopy(ptr, &v, sizeof(v));
}

internal void MemoryWrite64(void *ptr, U64 v)
{
	MemoryCopy(ptr, &v, sizeof(v));
}

internal U8 MemoryRead8(void *ptr)
{
	U8 result;
	MemoryCopy(&result, ptr, sizeof(result));
	return result;
}

internal U16 MemoryRead16(void *ptr)
{
	U16 result;
	MemoryCopy(&result, ptr, sizeof(result));
	return result;
}

internal U32 MemoryRead32(void *ptr)
{
	U32 result;
	MemoryCopy(&result, ptr, sizeof(result));
	return result;
}

internal U64 MemoryRead64(void *ptr)
{
	U64 result;
	MemoryCopy(&result, ptr, sizeof(result));
	return result;
}

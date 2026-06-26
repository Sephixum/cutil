#include "base_hash.h"
#include "base_core.h"

internal U64 HashBytes(void* data, U64 size)
{
	U8* bytes = (U8*)data;
	U64 hash  = 14695981039346656037ULL; // FNV offset basis
	for
		EachIndex(i, size)
		{
			hash ^= bytes[i];
			hash *= 1099511628211ULL; // FNV prime
		}
	return hash;
}

internal U64 HashU64(U64 x)
{
	x ^= x >> 30;
	x *= 0xbf58476d1ce4e5b9ULL;
	x ^= x >> 27;
	x *= 0x94d049bb133111ebULL;
	x ^= x >> 31;
	return x;
}

internal U64 HashStr8(String8 str)
{
	return HashBytes(str.str, str.size);
}

internal U64 HashU32(U32 x)
{
	return HashU64((U64)x);
}

internal U64 HashU16(U16 x)
{
	return HashU64((U64)x);
}

internal U64 HashU8(U8 x)
{
	return HashU64((U64)x);
}

internal U64 HashF32(F32 x)
{
	return HashBytes(&x, sizeof(x));
}

internal U64 HashF64(F64 x)
{
	return HashBytes(&x, sizeof(x));
}

#ifndef BASE_HASH_H
#define BASE_HASH_H

#include "base_typedefs.h"
#include "base_string.h"

////////////////////////////////
// FNV-1a over arbitrary bytes
// Use for String8 keys
internal U64 HashBytes(void* data, U64 size);

////////////////////////////////
// Murmur3 finalizer
// Scrambles sequential IDs so they don't
// cluster in the tablek
internal U64 HashU64(U64 x);

////////////////////////////////
// Convinience Wrappers
internal U64 HashStr8(String8 str);
internal U64 HashU32(U32 x);
internal U64 HashU16(U16 x);
internal U64 HashU8(U8 x);

internal U64 HashF32(F32 x);
internal U64 HashF64(F64 x);

#endif // BASE_HASH_H

#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

#include "base_typedefs.h"

internal void *MemoryReserve(U64 size);
internal B32   MemoryCommit(void *ptr, U64 size);
internal void  MemoryDecommit(void *ptr, U64 size);
internal void  MemoryRelease(void *ptr, U64 size);

internal void *MemoryReserveLarge(U64 size);
internal B32   MemoryCommitLarge(void *ptr, U64 size);

////////////////////////////////
// Memory Functions

internal B32 MemoryIsZero(void *ptr, U64 size);

internal void MemoryWrite16(void *ptr, U16 v);
internal void MemoryWrite32(void *ptr, U32 v);
internal void MemoryWrite64(void *ptr, U64 v);

internal U8	 MemoryRead8(void *ptr);
internal U16 MemoryRead16(void *ptr);
internal U32 MemoryRead32(void *ptr);
internal U64 MemoryRead64(void *ptr);

#endif // BASE_MEMORY_H

#include "base_threads.h"

#if COMPILER_MSVC
#include <intrin.h>

// ---- Loads ----
#define ins_load_u8(x) __iso_volatile_load8((const __int8*)(x))
#define ins_load_u16(x) __iso_volatile_load16((const __int16*)(x))
#define ins_load_u32(x) __iso_volatile_load32((const __int32*)(x))
#define ins_load_u64(x) __iso_volatile_load64((const __int64*)(x))

// ---- Exchanges ----
#define ins_exchange_u8(x, v) _InterlockedExchange8((volatile char*)(x), (char)(v))
#define ins_exchange_u16(x, v) _InterlockedExchange16((volatile short*)(x), (short)(v))
#define ins_exchange_u32(x, v) _InterlockedExchange((volatile long*)(x), (long)(v))
#define ins_exchange_u64(x, v) _InterlockedExchange64((volatile __int64*)(x), (__int64)(v))

// ---- Compare-And-Swap ----
#define ins_cas_u8(x, d, e) _InterlockedCompareExchange8((volatile char*)(x), (char)(d), (char)(e))
#define ins_cas_u16(x, d, e) _InterlockedCompareExchange16((volatile short*)(x), (short)(d), (short)(e))
#define ins_cas_u32(x, d, e) _InterlockedCompareExchange((volatile long*)(x), (long)(d), (long)(e))
#define ins_cas_u64(x, d, e) _InterlockedCompareExchange64((volatile __int64*)(x), (__int64)(d), (__int64)(e))

// ---- Fetch-And-Add (Only 32/64 have native add; 8/16 use CAS loop in wrapper) ----
#define ins_fetch_add_u32(x, v) _InterlockedExchangeAdd((volatile long*)(x), (long)(v))			// returns OLD
#define ins_fetch_add_u64(x, v) _InterlockedExchangeAdd64((volatile __int64*)(x), (__int64)(v)) // returns OLD

// ---- Fetch-And (Bitwise) ----
#define ins_fetch_and_u8(x, v) _InterlockedAnd8((volatile char*)(x), (char)(v))
#define ins_fetch_and_u16(x, v) _InterlockedAnd16((volatile short*)(x), (short)(v))
#define ins_fetch_and_u32(x, v) _InterlockedAnd((volatile long*)(x), (long)(v))
#define ins_fetch_and_u64(x, v) _InterlockedAnd64((volatile __int64*)(x), (__int64)(v))

#define ins_fetch_or_u8(x, v) _InterlockedOr8((volatile char*)(x), (char)(v))
#define ins_fetch_or_u16(x, v) _InterlockedOr16((volatile short*)(x), (short)(v))
#define ins_fetch_or_u32(x, v) _InterlockedOr((volatile long*)(x), (long)(v))
#define ins_fetch_or_u64(x, v) _InterlockedOr64((volatile __int64*)(x), (__int64)(v))

// ---- Fence ----
#define ins_fence() MemoryBarrier()

#elif COMPILER_GCC || COMPILER_CLANG
#include <immintrin.h>

// ---- Loads ----
#define ins_load_u8(x) __atomic_load_n((U8*)(x), __ATOMIC_SEQ_CST)
#define ins_load_u16(x) __atomic_load_n((U16*)(x), __ATOMIC_SEQ_CST)
#define ins_load_u32(x) __atomic_load_n((U32*)(x), __ATOMIC_SEQ_CST)
#define ins_load_u64(x) __atomic_load_n((U64*)(x), __ATOMIC_SEQ_CST)

// ---- Exchanges ----
#define ins_exchange_u8(x, v) __atomic_exchange_n((U8*)(x), (U8)(v), __ATOMIC_SEQ_CST)
#define ins_exchange_u16(x, v) __atomic_exchange_n((U16*)(x), (U16)(v), __ATOMIC_SEQ_CST)
#define ins_exchange_u32(x, v) __atomic_exchange_n((U32*)(x), (U32)(v), __ATOMIC_SEQ_CST)
#define ins_exchange_u64(x, v) __atomic_exchange_n((U64*)(x), (U64)(v), __ATOMIC_SEQ_CST)

// ---- Compare-And-Swap ----
// We wrap it to return the OLD value for compatibility with MSVC's _InterlockedCompareExchange.
#define ins_cas_u8(x, d, e)                                                                                            \
	({                                                                                                                 \
		U8 _e = (U8)(e);                                                                                               \
		__atomic_compare_exchange_n((U8*)(x), &_e, (U8)(d), 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST) ? _e : _e;          \
	})
#define ins_cas_u16(x, d, e)                                                                                           \
	({                                                                                                                 \
		U16 _e = (U16)(e);                                                                                             \
		__atomic_compare_exchange_n((U16*)(x), &_e, (U16)(d), 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST) ? _e : _e;        \
	})
#define ins_cas_u32(x, d, e)                                                                                           \
	({                                                                                                                 \
		U32 _e = (U32)(e);                                                                                             \
		__atomic_compare_exchange_n((U32*)(x), &_e, (U32)(d), 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST) ? _e : _e;        \
	})
#define ins_cas_u64(x, d, e)                                                                                           \
	({                                                                                                                 \
		U64 _e = (U64)(e);                                                                                             \
		__atomic_compare_exchange_n((U64*)(x), &_e, (U64)(d), 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST) ? _e : _e;        \
	})

// ---- Fetch-And-Add (returns OLD value) ----
#define ins_fetch_add_u8(x, v) __atomic_fetch_add((U8*)(x), (U8)(v), __ATOMIC_SEQ_CST)
#define ins_fetch_add_u16(x, v) __atomic_fetch_add((U16*)(x), (U16)(v), __ATOMIC_SEQ_CST)
#define ins_fetch_add_u32(x, v) __atomic_fetch_add((U32*)(x), (U32)(v), __ATOMIC_SEQ_CST)
#define ins_fetch_add_u64(x, v) __atomic_fetch_add((U64*)(x), (U64)(v), __ATOMIC_SEQ_CST)

// ---- Fetch-And (Bitwise) ----
#define ins_fetch_and_u8(x, v) __atomic_fetch_and((U8*)(x), (U8)(v), __ATOMIC_SEQ_CST)
#define ins_fetch_and_u16(x, v) __atomic_fetch_and((U16*)(x), (U16)(v), __ATOMIC_SEQ_CST)
#define ins_fetch_and_u32(x, v) __atomic_fetch_and((U32*)(x), (U32)(v), __ATOMIC_SEQ_CST)
#define ins_fetch_and_u64(x, v) __atomic_fetch_and((U64*)(x), (U64)(v), __ATOMIC_SEQ_CST)

#define ins_fetch_or_u8(x, v) __atomic_fetch_or((U8*)(x), (U8)(v), __ATOMIC_SEQ_CST)
#define ins_fetch_or_u16(x, v) __atomic_fetch_or((U16*)(x), (U16)(v), __ATOMIC_SEQ_CST)
#define ins_fetch_or_u32(x, v) __atomic_fetch_or((U32*)(x), (U32)(v), __ATOMIC_SEQ_CST)
#define ins_fetch_or_u64(x, v) __atomic_fetch_or((U64*)(x), (U64)(v), __ATOMIC_SEQ_CST)

// ---- Fence ----
#define ins_fence() __atomic_thread_fence(__ATOMIC_SEQ_CST)

#else
#error "Unsupported compiler for atomic operations!"
#endif

// ----------------------------------------------------------------
// 8-Bit Implementation
// ----------------------------------------------------------------
U8 Atomic_LoadU8(volatile U8* ptr)
{
	return ins_load_u8(ptr);
}

void Atomic_StoreU8(volatile U8* ptr, U8 val)
{
	ins_exchange_u8(ptr, val); // Exchange does a full atomic write (and ignores old)
}

U8 Atomic_ExchangeU8(volatile U8* ptr, U8 val)
{
	return ins_exchange_u8(ptr, val);
}

B32 Atomic_CasU8(volatile U8* ptr, U8 expected, U8 desired)
{
	return ins_cas_u8(ptr, desired, expected) == expected;
}

U8 Atomic_FetchAddU8(volatile U8* ptr, U8 val)
{
#if COMPILER_MSVC
	// MSVC lacks InterlockedAdd8. Use CAS loop.
	U8 old, new_val;
	do
	{
		old		= ins_load_u8(ptr);
		new_val = (U8)(old + val);
	} while (ins_cas_u8(ptr, new_val, old) != old);
	return old;
#else
	return ins_fetch_add_u8(ptr, val);
#endif
}

U8 Atomic_FetchAndU8(volatile U8* ptr, U8 val)
{
	return ins_fetch_and_u8(ptr, val);
}

U8 Atomic_FetchOrU8(volatile U8* ptr, U8 val)
{
	return ins_fetch_or_u8(ptr, val);
}

// ----------------------------------------------------------------
// 16-Bit Implementation
// ----------------------------------------------------------------
U16 Atomic_LoadU16(volatile U16* ptr)
{
	return ins_load_u16(ptr);
}

void Atomic_StoreU16(volatile U16* ptr, U16 val)
{
	ins_exchange_u16(ptr, val);
}

U16 Atomic_ExchangeU16(volatile U16* ptr, U16 val)
{
	return ins_exchange_u16(ptr, val);
}

B32 Atomic_CasU16(volatile U16* ptr, U16 expected, U16 desired)
{
	return ins_cas_u16(ptr, desired, expected) == expected;
}

U16 Atomic_FetchAddU16(volatile U16* ptr, U16 val)
{
#if COMPILER_MSVC
	// MSVC lacks InterlockedAdd16. Use CAS loop.
	U16 old, new_val;
	do
	{
		old		= ins_load_u16(ptr);
		new_val = (U16)(old + val);
	} while (ins_cas_u16(ptr, new_val, old) != old);
	return old;
#else
	return ins_fetch_add_u16(ptr, val);
#endif
}

U16 Atomic_FetchAndU16(volatile U16* ptr, U16 val)
{
	return ins_fetch_and_u16(ptr, val);
}

U16 Atomic_FetchOrU16(volatile U16* ptr, U16 val)
{
	return ins_fetch_or_u16(ptr, val);
}

// ----------------------------------------------------------------
// 32-Bit Implementation
// ----------------------------------------------------------------
U32 Atomic_LoadU32(volatile U32* ptr)
{
	return ins_load_u32(ptr);
}

void Atomic_StoreU32(volatile U32* ptr, U32 val)
{
	ins_exchange_u32(ptr, val);
}

U32 Atomic_ExchangeU32(volatile U32* ptr, U32 val)
{
	return ins_exchange_u32(ptr, val);
}

B32 Atomic_CasU32(volatile U32* ptr, U32 expected, U32 desired)
{
	return ins_cas_u32(ptr, desired, expected) == expected;
}

U32 Atomic_FetchAddU32(volatile U32* ptr, U32 val)
{
	return ins_fetch_add_u32(ptr, val);
}

U32 Atomic_FetchAndU32(volatile U32* ptr, U32 val)
{
	return ins_fetch_and_u32(ptr, val);
}

U32 Atomic_FetchOrU32(volatile U32* ptr, U32 val)
{
	return ins_fetch_or_u32(ptr, val);
}

// ----------------------------------------------------------------
// 64-Bit Implementation
// ----------------------------------------------------------------
U64 Atomic_LoadU64(volatile U64* ptr)
{
	return ins_load_u64(ptr);
}

void Atomic_StoreU64(volatile U64* ptr, U64 val)
{
	ins_exchange_u64(ptr, val);
}

U64 Atomic_ExchangeU64(volatile U64* ptr, U64 val)
{
	return ins_exchange_u64(ptr, val);
}

B32 Atomic_CasU64(volatile U64* ptr, U64 expected, U64 desired)
{
	return ins_cas_u64(ptr, desired, expected) == expected;
}

U64 Atomic_FetchAddU64(volatile U64* ptr, U64 val)
{
	return ins_fetch_add_u64(ptr, val);
}

U64 Atomic_FetchAndU64(volatile U64* ptr, U64 val)
{
	return ins_fetch_and_u64(ptr, val);
}

U64 Atomic_FetchOrU64(volatile U64* ptr, U64 val)
{
	return ins_fetch_or_u64(ptr, val);
}

// ----------------------------------------------------------------
// Pointer-Sized (UPtr) - Forward to 64-bit (or 32-bit if supported)
// ----------------------------------------------------------------
// Since you don't support 32-bit, we map UPtr directly to U64.
// If you ever support 32-bit, you'd need a conditional branch here.
UPtr Atomic_LoadUPtr(UPtr volatile* ptr)
{
	return (UPtr)Atomic_LoadU64((volatile U64*)ptr);
}

void Atomic_StoreUPtr(UPtr volatile* ptr, UPtr val)
{
	Atomic_StoreU64((volatile U64*)ptr, (U64)val);
}

UPtr Atomic_ExchangeUPtr(UPtr volatile* ptr, UPtr val)
{
	return (UPtr)Atomic_ExchangeU64((volatile U64*)ptr, (U64)val);
}

B32 Atomic_CasUPtr(UPtr volatile* ptr, UPtr expected, UPtr desired)
{
	return Atomic_CasU64((volatile U64*)ptr, (U64)expected, (U64)desired);
}

// ----------------------------------------------------------------
// 32-Bit Float (Bitwise cast via memcpy)
// ----------------------------------------------------------------
F32 Atomic_LoadF32(volatile F32* ptr)
{
	U32 bits = Atomic_LoadU32((volatile U32*)ptr);
	F32 result;
	memcpy(&result, &bits, sizeof(F32));
	return result;
}

void Atomic_StoreF32(volatile F32* ptr, F32 val)
{
	U32 bits;
	memcpy(&bits, &val, sizeof(F32));
	Atomic_StoreU32((volatile U32*)ptr, bits);
}

F32 Atomic_ExchangeF32(volatile F32* ptr, F32 val)
{
	U32 val_bits, old_bits;
	memcpy(&val_bits, &val, sizeof(F32));
	old_bits = Atomic_ExchangeU32((volatile U32*)ptr, val_bits);
	F32 old;
	memcpy(&old, &old_bits, sizeof(F32));
	return old;
}

B32 Atomic_CasF32(volatile F32* ptr, F32 expected, F32 desired)
{
	U32 expected_bits, desired_bits;
	memcpy(&expected_bits, &expected, sizeof(F32));
	memcpy(&desired_bits, &desired, sizeof(F32));
	return Atomic_CasU32((volatile U32*)ptr, expected_bits, desired_bits);
}

// ----------------------------------------------------------------
// 64-Bit Float (Bitwise cast via memcpy)
// ----------------------------------------------------------------
F64 Atomic_LoadF64(volatile F64* ptr)
{
	U64 bits = Atomic_LoadU64((volatile U64*)ptr);
	F64 result;
	memcpy(&result, &bits, sizeof(F64));
	return result;
}

void Atomic_StoreF64(volatile F64* ptr, F64 val)
{
	U64 bits;
	memcpy(&bits, &val, sizeof(F64));
	Atomic_StoreU64((volatile U64*)ptr, bits);
}

F64 Atomic_ExchangeF64(volatile F64* ptr, F64 val)
{
	U64 val_bits, old_bits;
	memcpy(&val_bits, &val, sizeof(F64));
	old_bits = Atomic_ExchangeU64((volatile U64*)ptr, val_bits);
	F64 old;
	memcpy(&old, &old_bits, sizeof(F64));
	return old;
}

B32 Atomic_CasF64(volatile F64* ptr, F64 expected, F64 desired)
{
	U64 expected_bits, desired_bits;
	memcpy(&expected_bits, &expected, sizeof(F64));
	memcpy(&desired_bits, &desired, sizeof(F64));
	return Atomic_CasU64((volatile U64*)ptr, expected_bits, desired_bits);
}

// ----------------------------------------------------------------
// Fence
// ----------------------------------------------------------------
void Atomic_Fence(void)
{
	ins_fence();
}

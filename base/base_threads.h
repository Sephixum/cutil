#ifndef BASE_THREADS_H
#define BASE_THREADS_H

#include "base_arena.h"
#include "base_core.h"
#include "base_typedefs.h"
#include "base_string.h"

////////////////////////////////
// Thread
typedef struct Thread Thread;
struct Thread
{
	U64 u64[1];
};
typedef void* (*ThreadProc)(void* arg);

internal Thread Thread_Make(ThreadProc proc, void* arg);
internal void	Thread_Join(Thread thread);
internal void	Thread_Detach(Thread thread);
internal void	Thread_Exit(void* retval);
internal Thread Thread_Current(void);
internal void	Thread_SetName(String8 name);
internal void	Thread_SetNameF(char* fmt, ...);

////////////////////////////////
// Mutex
typedef struct Mutex Mutex;
struct Mutex
{
	U64 u64[1];
};

internal Mutex Mutex_Make(void);
internal void  Mutex_Destroy(Mutex m);
internal void  Mutex_Lock(Mutex m);
internal void  Mutex_Unlock(Mutex m);

////////////////////////////////
// CondVar
typedef struct CondVar CondVar;
struct CondVar
{
	U64 u64[1];
};

internal CondVar CondVar_Make(void);
internal void	 CondVar_Destroy(CondVar cv);
internal void	 CondVar_Wait(CondVar cv, Mutex m);
internal void	 CondVar_Signal(CondVar cv);
internal void	 CondVar_Broadcast(CondVar cv);

///////////////////////////
// Atomic operations

// ---- 8-bit ----
internal U8	  Atomic_LoadU8(volatile U8* ptr);
internal void Atomic_StoreU8(volatile U8* ptr, U8 val);
internal U8	  Atomic_ExchangeU8(volatile U8* ptr, U8 val);
internal B32  Atomic_CasU8(volatile U8* ptr, U8 expected, U8 desired);
internal U8	  Atomic_FetchAddU8(volatile U8* ptr, U8 val);
internal U8	  Atomic_FetchAndU8(volatile U8* ptr, U8 val);
internal U8	  Atomic_FetchOrU8(volatile U8* ptr, U8 val);

// ---- 16-bit ----
internal U16  Atomic_LoadU16(volatile U16* ptr);
internal void Atomic_StoreU16(volatile U16* ptr, U16 val);
internal U16  Atomic_ExchangeU16(volatile U16* ptr, U16 val);
internal B32  Atomic_CasU16(volatile U16* ptr, U16 expected, U16 desired);
internal U16  Atomic_FetchAddU16(volatile U16* ptr, U16 val);
internal U16  Atomic_FetchAndU16(volatile U16* ptr, U16 val);
internal U16  Atomic_FetchOrU16(volatile U16* ptr, U16 val);

// ---- 32-bit ----
internal U32  Atomic_LoadU32(volatile U32* ptr);
internal void Atomic_StoreU32(volatile U32* ptr, U32 val);
internal U32  Atomic_ExchangeU32(volatile U32* ptr, U32 val);
internal B32  Atomic_CasU32(volatile U32* ptr, U32 expected, U32 desired);
internal U32  Atomic_FetchAddU32(volatile U32* ptr, U32 val);
internal U32  Atomic_FetchAndU32(volatile U32* ptr, U32 val);
internal U32  Atomic_FetchOrU32(volatile U32* ptr, U32 val);

// ---- 64-bit ----
internal U64  Atomic_LoadU64(volatile U64* ptr);
internal void Atomic_StoreU64(volatile U64* ptr, U64 val);
internal U64  Atomic_ExchangeU64(volatile U64* ptr, U64 val);
internal B32  Atomic_CasU64(volatile U64* ptr, U64 expected, U64 desired);
internal U64  Atomic_FetchAddU64(volatile U64* ptr, U64 val);
internal U64  Atomic_FetchAndU64(volatile U64* ptr, U64 val);
internal U64  Atomic_FetchOrU64(volatile U64* ptr, U64 val);

// ---- Pointers (Requires UPtr in base_typedefs.h) ----
internal UPtr Atomic_LoadUPtr(UPtr volatile* ptr);
internal void Atomic_StoreUPtr(UPtr volatile* ptr, UPtr val);
internal UPtr Atomic_ExchangeUPtr(UPtr volatile* ptr, UPtr val);
internal B32  Atomic_CasUPtr(UPtr volatile* ptr, UPtr expected, UPtr desired);

#define Atomic_LoadPtr(ptr) (void*)Atomic_LoadUPtr((volatile UPtr*)(ptr))
#define Atomic_StorePtr(ptr, val) Atomic_StoreUPtr((volatile UPtr*)(ptr), (UPtr)(val))
#define Atomic_CasPtr(ptr, expected, desired) Atomic_CasUPtr((volatile UPtr*)(ptr), (UPtr)(expected), (UPtr)(desired))
#define Atomic_ExchangePtr(ptr, desired) (void*)Atomic_ExchangeUPtr((volatile UPtr*)(ptr), (UPtr)(desired))

// ---- Floats (Load, Store, Exchange, CAS only. NO FETCH_ADD) ----
internal F32  Atomic_LoadF32(volatile F32* ptr);
internal void Atomic_StoreF32(volatile F32* ptr, F32 val);
internal F32  Atomic_ExchangeF32(volatile F32* ptr, F32 val);
internal B32  Atomic_CasF32(volatile F32* ptr, F32 expected, F32 desired);

internal F64  Atomic_LoadF64(volatile F64* ptr);
internal void Atomic_StoreF64(volatile F64* ptr, F64 val);
internal F64  Atomic_ExchangeF64(volatile F64* ptr, F64 val);
internal B32  Atomic_CasF64(volatile F64* ptr, F64 expected, F64 desired);

// ---- Fence ----
internal void Atomic_Fence(void);

#endif // BASE_THREADS_H

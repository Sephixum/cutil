#ifndef BASE_THREADS_H
#define BASE_THREADS_H

#include "base_arena.h"
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

internal Thread* Thread_Create(ThreadProc proc, void* arg);
internal void	 Thread_Join(Thread* thread);
internal void	 Thread_Detach(Thread* thread);
internal void	 Thread_Exit(void* retval);
internal Thread* Thread_Current(void);
internal void	 Thread_SetName(String8 name);
internal void	 Thread_SetNameF(char* fmt, ...);

////////////////////////////////
// Mutex
typedef struct Mutex Mutex;
struct Mutex
{
	U64 u64[1];
};

internal Mutex* Mutex_Create(void);
internal void	Mutex_Destory(Mutex* m);
internal void	Mutex_Lock(Mutex* m);
internal void	Mutex_Unlock(Mutex* m);

////////////////////////////////
// CondVar
typedef struct CondVar CondVar;
struct CondVar
{
	U64 u64[1];
};

internal CondVar* CondVar_Create(void);
internal void	  CondVar_Destroy(CondVar* cv);
internal void	  CondVar_Wait(CondVar* cv, Mutex* m);
internal void	  CondVar_Signal(CondVar* cv);
internal void	  CondVar_Broadcast(CondVar* cv);

///////////////////////////
// Atomic operations
internal U32  Atomic_LoadU32(volatile U32* ptr);
internal void Atomic_StoreU32(volatile U32* ptr, U32 val);
internal U32  Atomic_ExchangeU32(volatile U32* ptr, U32 val);
internal B32  Atomic_CasU32(volatile U32* ptr, U32 expected, U32 desired);
internal U32  Atomic_FetchAddU32(volatile U32* ptr, U32 val);
internal U32  Atomic_FetchSubU32(volatile U32* ptr, U32 val);

internal U64  Atomic_LoadU64(volatile U64* ptr);
internal void Atomic_StoreU64(volatile U64* ptr, U64 val);
internal U64  Atomic_ExchangeU64(volatile U64* ptr, U64 val);
internal B32  Atomic_CasU64(volatile U64* ptr, U64 expected, U64 desired);
internal U64  Atomic_FetchAddU64(volatile U64* ptr, U64 val);
internal U64  Atomic_FetchSubU64(volatile U64* ptr, U64 val);

// ---- Pointers (wrappers) ----
#define Atomic_CasPtr(ptr, expected, desired) Atomic_CasU64((volatile U64*)(ptr), (U64)(expected), (U64)(desired))
#define Atomic_ExchangePtr(ptr, desired) (void*)Atomic_ExchangeU64((volatile U64*)(ptr), (U64)(desired))

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

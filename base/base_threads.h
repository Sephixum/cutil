#ifndef BASE_THREADS_H
#define BASE_THREADS_H

#include "base_arena.h"
#include "base_typedefs.h"
#include "base_string.h"

////////////////////////////////
// Thread Types

typedef struct Thread Thread;
struct Thread
{
	U64 u64[1];
};
typedef void ThreadEntryPointFunctionType(void *p);

////////////////////////////////
//~ rjf: Synchronization Primitive Types

typedef struct Mutex Mutex;
struct Mutex
{
	U64 u64[1];
};

typedef struct RWMutex RWMutex;
struct RWMutex
{
	U64 u64[1];
};

typedef struct CondVar CondVar;
struct CondVar
{
	U64 u64[1];
};

typedef struct Semaphore Semaphore;
struct Semaphore
{
	U64 u64[1];
};

typedef struct Barrier Barrier;
struct Barrier
{
	U64 u64[1];
};

////////////////////////////////
//~ rjf: Table Stripes

typedef struct Stripe Stripe;
struct Stripe
{
	Arena  *arena;
	RWMutex rw_mutex;
	CondVar cv;
	void   *free;
};

typedef struct StripeArray StripeArray;
struct StripeArray
{
	Stripe *v;
	U64		count;
};

////////////////////////////////
// Table Stripe Functions

internal StripeArray StripeArrayAlloc(Arena *arena);
internal void		 StripeArrayRelease(StripeArray *stripes);
internal Stripe		*StripeFromSlotIndex(StripeArray *stripes, U64 slot_idx);

////////////////////////////////
// Thread Info Helpers

internal void SetThreadName(String8 string);
internal void SetThreadNameF(char *fmt, ...);

////////////////////////////////
// @per_os_impl Current Thread Info

internal U32  ThreadID(void);
internal void SetPlatformthreadName(String8 name);

////////////////////////////////
// @per_os_impl Thread Functions

internal Thread ThreadLaunch(ThreadEntryPointFunctionType *f, void *p);
internal B32	ThreadJoin(Thread thread, U64 endt_us);
internal void	ThreadDetach(Thread thread);

////////////////////////////////
// @per_os_impl Synchronization Primitive Functions

// recursive mutexes
internal Mutex MutexAlloc(void);
internal void  MutexRelease(Mutex mutex);
internal void  MutexTake(Mutex mutex);
internal void  MutexDrop(Mutex mutex);

// reader/writer mutexes
internal RWMutex RWMutexAlloc(void);
internal void	 RWMutexRelease(RWMutex mutex);
internal void	 RWMutexTake(RWMutex mutex, B32 write_mode);
internal void	 RWMutexDrop(RWMutex mutex, B32 write_mode);
#define RWMutexTakeR(m) RWMutexTake((m), (0))
#define RWMutexTakeW(m) RWMutexTake((m), (1))
#define RWMutexDropR(m) RWMutexDrop((m), (0))
#define RWMutexDropW(m) RWMutexDrop((m), (1))

// condition variables
internal CondVar CondVarAlloc(void);
internal void	 CondVarRelease(CondVar cv);
// returns false on timeout, true on signal, (max_wait_ms = max_U64) -> no timeout
internal B32 CondVarWait(CondVar cv, Mutex mutex, U64 endt_us);
internal B32 CondVarWaitRW(CondVar cv, RWMutex mutex_rw, B32 write_mode, U64 endt_us);
#define CondVarWaitRWR(cv, m, endt) CondVarWaitRW((cv), (m), (0), (endt))
#define CondVarWaitRWW(cv, m, endt) CondVarWaitRW((cv), (m), (1), (endt))
internal void CondVarSignal(CondVar cv);
internal void CondVarBroadcast(CondVar cv);

// cross-process semaphores
internal Semaphore SemaphoreAlloc(U32 initial_count, U32 max_count, String8 name);
internal void	   SemaphoreRelease(Semaphore semaphore);
internal Semaphore SemaphoreOpen(String8 name);
internal void	   SemaphoreClose(Semaphore semaphore);
internal B32	   SemaphoreTake(Semaphore semaphore, U64 endt_us);
internal void	   SemaphoreDrop(Semaphore semaphore);

// barriers
internal Barrier BarrierAlloc(U64 count);
internal void	 BarrierRelease(Barrier barrier);
internal void	 BarrierWait(Barrier barrier);

// scope macros
#define MutexScope(mutex) DeferLoop(MutexTake(mutex), MutexDrop(mutex))
#define RWMutexScope(mutex, write_mode)                                                                                \
	DeferLoop(RWMutexTake((mutex), (write_mode)), RWMutexDrop((mutex), (write_mode)))
#define MutexScopeR(mutex) DeferLoop(RWMutexTakeR(mutex), RWMutexDropR(mutex))
#define MutexScopeW(mutex) DeferLoop(RWMutexTakeW(mutex), RWMutexDropW(mutex))
#define MutexScopeRWPromote(mutex)                                                                                     \
	DeferLoop((RWMutexDropR(mutex), RWMutexTakeW(mutex)), (RWMutexDropW(mutex), RWMutexTakeR(mutex)))

////////////////////////////////
// Platform-Abstracted Synchronization Primitive Functions

// slow barriers
internal Barrier SlowBarrierAlloc(U64 count);
internal void	 SlowBarrierRelease(Barrier barrier);
internal void	 SlowBarrierWait(Barrier barrier);

////////////////////////////////
// @per_os_impl Safe Calls

internal void SafeCall(ThreadEntryPointFunctionType *func, ThreadEntryPointFunctionType *fail_handler, void *ptr);

#endif // BASE_THREADS_H

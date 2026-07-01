///////////////////////////
// Thread
#include "../../base_threads.h"
#include "../../base_arena.h"
#include "base_include.h"
#include <threads.h>
#include <pthread.h>

internal Arena* lnx_arena = 0;

typedef struct OsThread OsThread;
struct OsThread
{
	pthread_t  handle;
	ThreadProc proc;
	void*	   arg;
};

typedef struct OsMutex OsMutex;
struct OsMutex
{
	pthread_mutex_t handle;
};

typedef struct OsCond OsCond;
struct OsCond
{
	pthread_cond_t handle;
};

internal thread_local OsThread* current_thread = 0;

internal void* _OsThreadEntry(void* p)
{
	OsThread* t	   = p;
	current_thread = t;
	return t->proc(t->arg);
}

internal Thread Thread_Make(ThreadProc proc, void* arg)
{
	if (lnx_arena == 0)
	{
		lnx_arena = Arena_Make();
	}

	OsThread* t = Arena_PushArray(lnx_arena, OsThread, 1);
	t->proc		= proc;
	t->arg		= arg;

	pthread_attr_t attr = {0};
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	int res = pthread_create(&t->handle, &attr, _OsThreadEntry, t);
	pthread_attr_destroy(&attr);

	if (res != 0)
	{
		return (Thread){0}; // Invalid handle (zero).
	}

	return (Thread){(U64)t};
}

internal void Thread_Join(Thread thread)
{
	if (*thread.u64 == 0)
	{
		return;
	}
	OsThread* t = (OsThread*)thread.u64[0];
	pthread_join(t->handle, 0);
	t->handle = 0;
}

internal void Thread_Detach(Thread thread)
{
	if (thread.u64[0] == 0)
	{
		return;
	}
	OsThread* t = (OsThread*)thread.u64[0];
	pthread_detach(t->handle);
}

internal void Thread_Exit(void* retval)
{
	pthread_exit(retval);
}

internal Thread Thread_Current(void)
{
	if (current_thread)
	{
		return (Thread){(U64)current_thread};
	}

	OsThread* t	   = Arena_PushArray(lnx_arena, OsThread, 1);
	t->handle	   = pthread_self();
	t->proc		   = NULL;
	t->arg		   = NULL;
	current_thread = t;
	return (Thread){(U64)t};
}

void Thread_SetName(String8 name)
{
	OsThread* t = current_thread;
	if (!t)
	{
		return;
	}

	char   buf[16];
	size_t len = (name.size < 15) ? name.size : 15;
	MemoryCopy(buf, name.str, len);
	buf[len] = '\0';
	pthread_setname_np(t->handle, buf);
}

internal void Thread_SetNameF(char* fmt, ...)
{
	OsThread* t = current_thread;
	if (!t)
	{
		return;
	}

	char	buf[16];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	pthread_setname_np(t->handle, buf);
}

//---------------------------------------------------------------
// Mutex API
//---------------------------------------------------------------
Mutex Mutex_Make(void)
{
	if (lnx_arena == NULL)
	{
		lnx_arena = Arena_Make();
	}

	OsMutex*			m = Arena_PushArray(lnx_arena, OsMutex, 1);
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
	pthread_mutex_init(&m->handle, &attr);
	pthread_mutexattr_destroy(&attr);
	return (Mutex){(U64)m};
}

void Mutex_Destroy(Mutex mutex)
{
	if (mutex.u64[0] == 0)
	{
		return;
	}
	OsMutex* m = (OsMutex*)mutex.u64[0];
	pthread_mutex_destroy(&m->handle);
	// Memory stays allocated in lnx_arena until OS_Shutdown().
}

void Mutex_Lock(Mutex mutex)
{
	if (mutex.u64[0] == 0)
	{
		return;
	}
	OsMutex* m = (OsMutex*)mutex.u64[0];
	pthread_mutex_lock(&m->handle);
}

void Mutex_Unlock(Mutex mutex)
{
	if (mutex.u64[0] == 0)
	{
		return;
	}
	OsMutex* m = (OsMutex*)mutex.u64[0];
	pthread_mutex_unlock(&m->handle);
}

//---------------------------------------------------------------
// CondVar API
//---------------------------------------------------------------
CondVar CondVar_Make(void)
{
	if (lnx_arena == NULL)
	{
		lnx_arena = Arena_Make();
	}

	OsCond* c = Arena_PushArray(lnx_arena, OsCond, 1);
	pthread_cond_init(&c->handle, NULL);
	return (CondVar){(U64)c};
}

void CondVar_Destroy(CondVar cv)
{
	if (cv.u64[0] == 0)
	{
		return;
	}
	OsCond* c = (OsCond*)cv.u64[0];
	pthread_cond_destroy(&c->handle);
}

void CondVar_Wait(CondVar cv, Mutex mutex)
{
	if (cv.u64[0] == 0 || mutex.u64[0] == 0)
	{
		return;
	}
	OsCond*	 c = (OsCond*)cv.u64[0];
	OsMutex* m = (OsMutex*)mutex.u64[0];
	pthread_cond_wait(&c->handle, &m->handle);
}

void CondVar_Signal(CondVar cv)
{
	if (cv.u64[0] == 0)
	{
		return;
	}
	OsCond* c = (OsCond*)cv.u64[0];
	pthread_cond_signal(&c->handle);
}

void CondVar_Broadcast(CondVar cv)
{
	if (cv.u64[0] == 0)
	{
		return;
	}
	OsCond* c = (OsCond*)cv.u64[0];
	pthread_cond_broadcast(&c->handle);
}

#include "threading.h"

#ifdef _WIN32

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct internalParams
{
    ThreadFunc *f;
    void *data;
} InternalParams;

static DWORD WINAPI InternalMethodPtr(LPVOID arg)
{
    InternalParams *params = (InternalParams *)arg;
    params->f(params->data);
    free(params);
    return 0;
}

int ThreadCreate(Thread *t, ThreadFunc f, void *data)
{
    InternalParams *params = malloc(sizeof(InternalParams));
    assert(params != NULL);
    params->f = f;
    params->data = data;
    *t = CreateThread(NULL, 0, InternalMethodPtr, params, 0, NULL);
    if (t == NULL)
    {
        free(params);
        return false;
    }
    return true;
}

int ThreadJoin(Thread *t)
{
    if (WaitForSingleObject(*t, INFINITE) != WAIT_FAILED)
    {
        if (CloseHandle(*t))
        {
            return true;
        }
    }
    return false;
}

int MutexInit(Mutex *m)
{
    InitializeCriticalSection(m);
    return true;
}

int MutexLock(Mutex *m)
{
    EnterCriticalSection(m);
    return true;
}

int MutexTryLock(Mutex *m)
{
    return !TryEnterCriticalSection(m);
}

int MutexUnlock(Mutex *m)
{
    LeaveCriticalSection(m);
    return true;
}

int MutexDestroy(Mutex *m)
{
    DeleteCriticalSection(m);
    return true;
}

int ConditionInit(Condition *c)
{
    InitializeConditionVariable(c);
    return true;
}

int ConditionSignal(Condition *c)
{
    WakeConditionVariable(c);
    return true;
}

int ConditionWait(Condition *c, Mutex *m)
{
    return !SleepConditionVariableCS(c, m, INFINITE);
}

int ConditionDestroy(Condition *c)
{
    return true;
}

#else

int ThreadCreate(Thread *t, ThreadFunc f, void *data)
{
    return pthread_create(t, 0, (void*)f, data);
}

int ThreadJoin(Thread *t)
{
    return pthread_join(t, NULL);
}

int MutexInit(Mutex *m)
{
    return pthread_mutex_init(m, NULL);
}

int MutexLock(Mutex *m)
{
    return pthread_mutex_lock(m);
}

int MutexTryLock(Mutex *m)
{
    return pthread_mutex_trylock(m);
}

int MutexUnlock(Mutex *m)
{
    return pthread_mutex_unlock(m);
}

int MutexDestroy(Mutex *m)
{
    return pthread_mutex_destroy(m);
}

int ConditionInit(Condition *c)
{
    return pthread_cond_init(c, NULL);
}

int ConditionSignal(Condition *c)
{
    return pthread_cond_signal(c);
}

int ConditionWait(Condition *c, Mutex *m)
{
    return pthread_cond_wait(c, m);
}

int ConditionDestroy(Condition *c)
{
    return pthread_cond_destroy(c);
}

#endif

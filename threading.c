#include "threading.h"

#ifdef _WIN32

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct internalParams
{
    ThreadFunc f;
    void *data;
} InternalParams;

static DWORD WINAPI InternalMethodPtr(LPVOID arg)
{
    InternalParams *params = (InternalParams *)arg;
    params->f(params->data);
    return 0;
}

int ThreadCreate(Thread *t, ThreadFunc f, void *data)
{
    InternalParams params;
    params.f = f;
    params.data = data;
    *t = CreateThread(NULL, 0, InternalMethodPtr, &params, 0, NULL);
    if (t == NULL)
        return false;
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
#endif

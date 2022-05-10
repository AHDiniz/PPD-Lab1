#ifndef THREADING_H_

#define THREADING_H_

#ifdef _WIN32
    #include <windows.h>
    #include <Windows.h>
    typedef HANDLE Thread;
    typedef CRITICAL_SECTION Mutex;
    typedef CONDITION_VARIABLE Condition;
#else
    #include <pthread.h>
    #include <sys/sysinfo.h>
    typedef pthread_t Thread;
    typedef pthread_mutex_t Mutex;
    typedef pthread_cond_t Condition;
#endif

typedef void (ThreadFunc) (void *);

int GetCoreCount(void);

int ThreadCreate(Thread *t, ThreadFunc f, void *data);
int ThreadJoin(Thread *t);

int MutexInit(Mutex *m);
int MutexLock(Mutex *m);
int MutexTryLock(Mutex *m);
int MutexUnlock(Mutex *m);
int MutexDestroy(Mutex *m);

int ConditionInit(Condition *c);
int ConditionSignal(Condition *c);
int ConditionWait(Condition *c, Mutex *m);
int ConditionDestroy(Condition *c);

#endif

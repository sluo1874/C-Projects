#include "t_lib.h"
#include <ucontext.h>
#include <unistd.h>

struct tcb* runningThread;
struct tcb* readyLowHead;
struct tcb* readyLowTail;
struct tcb* readyHighHead;
struct tcb* readyHighTail;

int init = 0;

void t_yield() {
    ualarm(0, 0);
    if (readyLowHead == readyLowTail && readyHighHead == readyHighTail) {
        ualarm(1, 0);
        return;
    }
    int low = 0;
    struct tcb* nextToRun = NULL;
    if (readyHighTail == readyHighHead) {
        nextToRun = readyLowHead->next;
        if (nextToRun == readyLowTail) {
            readyLowTail = readyLowHead;
        }
        readyLowHead->next = nextToRun->next;
    } else {
        nextToRun = readyHighHead->next;
        if (nextToRun == readyHighTail) {
            readyHighTail = readyHighHead;
        }
        readyHighHead->next = nextToRun->next;
    }
    
    if (runningThread->thread_priority == 1) {
        readyLowTail->next = runningThread;
        readyLowTail = readyLowTail->next;
        low = 1;
    } else {
        readyHighTail->next = runningThread;
        readyHighTail = readyHighTail->next;
        low = 0;
    }
    
    nextToRun->next = NULL;
    runningThread = nextToRun;
    ualarm(1, 0);
    if (low) {
        swapcontext(readyLowTail->thread_context, runningThread->thread_context);
    } else {
        swapcontext(readyHighTail->thread_context, runningThread->thread_context);
    }
}

void catchAlarm(int sig) {
    t_yield();
    signal(SIGALRM, catchAlarm);
}

void t_init() {
    if (init) {
        return;
    }
    signal(SIGALRM, catchAlarm);
    readyLowHead = (struct tcb*) malloc( sizeof(struct tcb));
    readyLowHead->next = NULL;
    readyLowTail = readyLowHead;
    
    readyHighHead = (struct tcb*) malloc( sizeof(struct tcb));
    readyHighHead->next = NULL;
    readyHighTail = readyHighHead;
    
    runningThread = (struct tcb*) malloc( sizeof(struct tcb));
    runningThread->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
    runningThread->thread_priority = 1;
    runningThread->next = NULL;
    
    getcontext(runningThread->thread_context);    /* let tmp be the context of main() */
    
    init = 1;
    ualarm(1, 0);
}

int t_create(void (*fct)(int), int id, int pri) {
    if (!init) {
        return -1;
    }
    
    size_t sz = 0x10000;
    struct tcb* newTcb = (struct tcb*) malloc(sizeof(struct tcb));
    if (pri == 1) {
        readyLowTail->next = newTcb;
        readyLowTail = readyLowTail->next;
    } else {
        readyHighTail->next = newTcb;
        readyHighTail = readyHighTail->next;
    }
    
    newTcb->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
    newTcb->next = NULL;
    newTcb->thread_id = id;
    newTcb->thread_priority = pri;
    ucontext_t* uc = newTcb->thread_context;
    getcontext(uc);
    /***
     uc->uc_stack.ss_sp = mmap(0, sz,
     PROT_READ | PROT_WRITE | PROT_EXEC,
     MAP_PRIVATE | MAP_ANON, -1, 0);
     ***/
    uc->uc_stack.ss_sp = malloc(sz);  /* new statement */
    uc->uc_stack.ss_size = sz;
    uc->uc_stack.ss_flags = 0;
    uc->uc_link = runningThread->thread_context;
    makecontext(uc, fct, 1, id);
    return 0;
}

void t_terminate(void) {
    ualarm(0, 0);
    if (readyLowHead == readyLowTail && readyHighHead == readyHighTail) {
        ualarm(1, 0);
        return;
    }
    int low = 0;
    struct tcb* nextToRun = NULL;
    if (readyHighTail == readyHighHead) {
        nextToRun = readyLowHead->next;
        if (nextToRun == readyLowTail) {
            readyLowTail = readyLowHead;
        }
        readyLowHead->next = nextToRun->next;
        free(runningThread->thread_context);
        free(runningThread);
        low = 1;
    } else {
        nextToRun = readyHighHead->next;
        if (nextToRun == readyHighTail) {
            readyHighTail = readyHighHead;
        }
        readyHighHead->next = nextToRun->next;
        free(runningThread->thread_context);
        free(runningThread);
        low = 0;
    }
    
    nextToRun->next = NULL;
    runningThread = nextToRun;
    ualarm(1, 0);
    setcontext(runningThread->thread_context);
}

void t_shutdown(void) {
    ualarm(0, 0);
    struct tcb * cursor = readyLowHead->next;
    struct tcb * tmp = NULL;
    while (cursor) {
        free(cursor->thread_context);
        tmp = cursor;
        cursor = cursor->next;
        free(tmp);
    }
    free(readyLowHead);
    
    
    cursor = readyHighHead->next;
    while (cursor) {
        free(cursor->thread_context);
        tmp = cursor;
        cursor = cursor->next;
        free(tmp);
    }
    free(readyHighHead);
    init = 0;
}

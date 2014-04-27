/*
 * types used by thread library
 */
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/mman.h>


void t_yield(void);
void t_init(void);
int t_create(void (*fct)(int), int id, int pri);
struct tcb {
    int         thread_id;
    int         thread_priority;
    ucontext_t*  thread_context;
    struct tcb *next;
};
void t_terminate(void);
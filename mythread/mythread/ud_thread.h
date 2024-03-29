/* 
 * thread library function prototypes
 */
#include "t_lib.h"

void t_create(void(*function)(int), int thread_id, int priority);
void t_yield(void);
void t_init(void);
void t_terminate(void);
void t_shutdown(void);
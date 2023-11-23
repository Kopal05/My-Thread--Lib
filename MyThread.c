#include <stdio.h>
#include "setjmp.h"
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "MyThread.h"
#define SECOND 1
#define STACK_SIZE 4096

static int currthread = -1;
static int nthread = 0;
static TCB threadlist[maxthread];
static int ran = 0;
static void wrapper();
void alarm_handler(int sig);
void initstatistics(struct statistics* stat,int id);
void clean();
void dispatch(int sig);
void yield();
void deletethread(int threadid);
int createwithargs(void *(*f)(void *),void *args);
int create(void (*f)(void));
int getid();
void run(int tid);
void suspend(int tid);
void sleep1(int secs);
void start(void);

#ifdef __x86_64__
typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7

address_t translate_address(address_t addr){
  address_t ret;
asm volatile("xor %%fs:0x30,%0\n"
  "rol       $0x11,%0\n"
             : "=g" (ret)
             : "0" (addr));
   return ret;
}
#endif

void initstatistics(struct statistics* stat,int d)


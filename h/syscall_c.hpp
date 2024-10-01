//
// Created by os on 7/29/23.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_HPP
#define PROJECT_BASE_V1_1_SYSCALL_C_HPP


#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/_Semaphore.hpp"

void* mem_alloc (size_t size);

int mem_free (void* ptr);


typedef  TCB  _thread ;
typedef _thread* thread_t;
int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);

int thread_exit ();

void thread_dispatch ();

void thread_join (
        thread_t handle
);

typedef _Semaphore _sem;
typedef _sem* sem_t;
int sem_open (
        sem_t* handle,
        unsigned init
);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);


typedef unsigned long time_t;
int time_sleep (time_t);

const int EOF = -1;
char getc ();

void putc (char c );

#endif //PROJECT_BASE_V1_1_SYSCALL_C_HPP

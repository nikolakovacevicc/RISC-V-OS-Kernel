//
// Created by os on 7/29/23.
//

#include "../h/syscall_c.hpp"
#include "../h/MemoryAllocator.hpp"


void* mem_alloc (size_t size){
    void * volatile  ptr ;
    __asm__ volatile("mv a1, %0" : : "r" (size));
    __asm__ volatile("mv a0, %0" : : "r" (0x01));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r" (ptr));
    return ptr;
}

int mem_free (void* ptr){
    int a;
    __asm__ volatile("mv a1, %0" : : "r" (ptr));
    __asm__ volatile("mv a0, %0" : : "r" (0x02));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r" (a));
    return a ;
}

int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
){
    volatile void* stack;
    if(start_routine == nullptr) stack = nullptr;
    else {
        //stack = mem_alloc(DEFAULT_STACK_SIZE);
        stack=mem_alloc(DEFAULT_STACK_SIZE);
        if(stack == nullptr) return -1;
    }


    __asm__ volatile("mv a0, %0" : : "r" (0x11):"a0");
    __asm__ volatile("mv a1, %0" : : "r" (handle):"a1");
    __asm__ volatile("mv a2, %0" : : "r" (start_routine):"a2");
    __asm__ volatile("mv a6, %0" : : "r" (arg):"a6");
    __asm__ volatile("mv a4, %0" : : "r" (stack):"a4");

    /*uint64 value = 0x11;
    asm volatile ("li a0, %0" : : "i" (value));*/

    __asm__ volatile("ecall");
    int a;
    __asm__ volatile("mv %0, a0" : "=r" (a));
    return a ;
}

int thread_exit (){
    int a;
    __asm__ volatile("mv a0, %0" : : "r" (0x12));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r" (a));
    return a ;
}

void thread_dispatch (){
    __asm__ volatile("mv a0, %0" : : "r" (0x13));
    __asm__ volatile("ecall");
}

void thread_join (
        thread_t handle
){
    __asm__ volatile("mv a0, %0" : : "r" (0x14):"a0");
    __asm__ volatile("mv a1, %0" : : "r" (handle):"a1");
    __asm__ volatile("ecall");
}

int sem_open (
        sem_t* handle,
        unsigned init
){
    __asm__ volatile("mv a0, %0" : : "r" (0x21):"a0");
    __asm__ volatile("mv a1, %0" : : "r" (handle):"a1");
    __asm__ volatile("mv a2, %0" : : "r" (init):"a2");

    __asm__ volatile("ecall");
    int a;
    __asm__ volatile("mv %0, a0" : "=r" (a));
    return a ;

}

int sem_close (sem_t handle){
    __asm__ volatile("mv a0, %0" : : "r" (0x22):"a0");
    __asm__ volatile("mv a1, %0" : : "r" (handle):"a1");
    __asm__ volatile("ecall");
    int a;
    __asm__ volatile("mv %0, a0" : "=r" (a));
    return a ;
}

int sem_wait (sem_t id){
    __asm__ volatile("mv a0, %0" : : "r" (0x23):"a0");
    __asm__ volatile("mv a1, %0" : : "r" (id):"a1");
    __asm__ volatile("ecall");
    int a;
    __asm__ volatile("mv %0, a0" : "=r" (a));
    return a ;
}

int sem_signal (sem_t id){
    __asm__ volatile("mv a0, %0" : : "r" (0x24):"a0");
    __asm__ volatile("mv a1, %0" : : "r" (id):"a1");
    __asm__ volatile("ecall");
    int a;
    __asm__ volatile("mv %0, a0" : "=r" (a));
    return a ;
}


int time_sleep (time_t){
    return 0;
}

char getc(){

    __asm__ volatile("mv a0, %0" : : "r" (0x41):"a0");
    __asm__ volatile("ecall");
    char a;
    __asm__ volatile("mv %0, a0" : "=r" (a));
    return a ;
}

void putc (char c){
    __asm__ volatile("mv a0, %0" : : "r" (0x42):"a0");
    __asm__ volatile("mv a1, %0" : : "r" (c):"a1");
    __asm__ volatile("ecall");
}
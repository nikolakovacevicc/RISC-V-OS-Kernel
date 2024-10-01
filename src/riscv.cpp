
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../h/_Semaphore.hpp"
#include "../h/printing.hpp"
void Riscv::popSppSpie()
{
    if(TCB::running->getId()!=1){
        mc_sstatus(SSTATUS_SPP);
    }
    __asm__ volatile ("csrw sepc,ra");
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap(){
    //uint64 volatile scause = r_scause();
    uint64 volatile sepc = r_sepc();
    uint64 volatile sstatus = r_sstatus();
    //pushRegisters();
    void* volatile stack;
    __asm__ volatile("mv %0, a4" : "=r" (stack));

    //if (scause == 0x0000000000000009UL || scause == 0x0000000000000008UL) {
        //sepc+=4;
        uint64 volatile code;
        __asm__ volatile("mv %0, a0" : "=r" (code));
        if (code == 0x01) {
            uint64 size;
            __asm__ volatile("mv %0, a1" : "=r" (size));

            size = size / MEM_BLOCK_SIZE + (size % MEM_BLOCK_SIZE > 0 ? 1 : 0);

            void *ptr = MemoryAllocator::_mem_alloc(size);
            __asm__ volatile("mv a0, %0" : : "r" (ptr));

        } else if (code == 0x02) {
            void *ptr1;
            __asm__ volatile("mv %0, a1" : "=r" (ptr1));
            int rez = MemoryAllocator::_mem_free(ptr1);
            __asm__ volatile("mv a0, %0" : : "r" (rez));

        } else if (code == 0x11) {
            thread_t *nit;
            __asm__ volatile("mv %0, a1" : "=r" (nit));

            void (*start_routine)(void *);
            __asm__ volatile("mv %0, a2" : "=r" (start_routine));

            void *arg;
            __asm__ volatile("mv %0, a6" : "=r" (arg));

            int rez;

            rez = TCB::thread_create(nit, start_routine, arg, stack);

            __asm__ volatile("mv a0, %0" : : "r" (rez));

        } else if (code == 0x12) {
            int rez = TCB::exitThread();
            __asm__ volatile("mv a0, %0" : : "r" (rez));
        } else if (code == 0x13) {
            //pushRegisters();
            TCB::dispatch();
            //popRegisters();
        } else if (code == 0x14) {

            thread_t nit;
            __asm__ volatile("mv %0, a1" : "=r" (nit));
            TCB::join(nit);

        } else if (code == 0x21) {
            sem_t *sem;
            __asm__ volatile("mv %0, a1" : "=r" (sem));

            unsigned init;
            __asm__ volatile("mv %0, a2" : "=r" (init));

            int rez = _Semaphore::sem_open(sem, init);
            __asm__ volatile("mv a0, %0" : : "r" (rez));
        } else if (code == 0x22) {
            sem_t sem;
            __asm__ volatile("mv %0, a1" : "=r" (sem));
            int rez = sem->sem_close();
            __asm__ volatile("mv a0, %0" : : "r" (rez));
        } else if (code == 0x23) {
            sem_t sem;
            __asm__ volatile("mv %0, a1" : "=r" (sem));
            int rez = sem->wait();
            __asm__ volatile("mv a0, %0" : : "r" (rez));
        } else if (code == 0x24) {
            sem_t sem;
            __asm__ volatile("mv %0, a1" : "=r" (sem));
            int rez = sem->signal();
            __asm__ volatile("mv a0, %0" : : "r" (rez));
        } else if (code == 0x41) {
            char rez = __getc();
            __asm__ volatile("mv a0, %0" : : "r" (rez));
        } else if (code == 0x42) {
            char c;
            __asm__ volatile("mv %0, a1" : "=r" (c));
            __putc(c);
        }
    /*}else
    if (scause == 0x8000000000000001UL ) {
    //timer
        mc_sip(Riscv::SIP_SSIP);
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            __putc(TCB::running->getId());
            TCB::timeSliceCounter = 0;
            TCB::dispatch();

        }

    }else
    if (scause == 0x8000000000000009UL ) {
        console_handler();

    }else{
        printS("panika\n");
        printI(r_scause());
        printS("sstatus:\n");
        printI(r_sstatus());
    }*/
    //popRegisters();
    w_sepc(sepc);
    w_sstatus(sstatus);
}

void Riscv::handleTimerTrap(){
    mc_sip(Riscv::SIP_SSIP);
}

void Riscv::handleConsoleTrap(){
    console_handler();
}
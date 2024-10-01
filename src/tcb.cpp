//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"

uint64 TCB::ID = 0;

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

int TCB::thread_create(TCB::thread_t *handle, void (*start_routine)(void *), void *arg, void *stack_space) {
    TCB *t = (TCB *) MemoryAllocator::_mem_alloc(sizeof(TCB)/MEM_BLOCK_SIZE + (sizeof(TCB)%MEM_BLOCK_SIZE ? 1:0));
    if (t == nullptr) return -1;
    t->state = CREATED;
    t->body = start_routine;
    t->arg = arg;
    t->stack =(uint64 *) stack_space;
    t->context.ra = (uint64) &threadWrapper;
    t->context.sp = t->stack != nullptr ? (uint64) &t->stack[DEFAULT_STACK_SIZE - 1] : 0;
    //t->context.sepc = (uint64) &threadWrapper;
    //t->timeSlice = DEFAULT_TIME_SLICE;
    t->timeSlice = 5;
    t->finished = false;
    t->id = ID++;
     _Semaphore::sem_open(&t->semafor,0);
    if(t->body!= nullptr)Scheduler::put(t);

    //*t=TCB(start_routine,stack_space,arg,DEFAULT_TIME_SLICE);
    *handle = t;
    return 0;
}

int TCB::exitThread() {
    //finsihed and dispatch and dealloc
    running->state = FINISHED;
    running->semafor->sem_close();
    MemoryAllocator::_mem_free(running->stack);
    dispatch();
    return 0;

}


void TCB::dispatch() {
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked()) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);

}

void TCB::yield() {
    __asm__ volatile("ecall");
}

void TCB::threadWrapper() {

    Riscv::popSppSpie();

    //running->state=RUNNING;
    running->body(running->arg);
    running->setFinished();
    thread_exit();

}

void TCB::wait(){
    this->semafor->wait();
}

void TCB::join(thread_t handle){
    handle->wait();
}

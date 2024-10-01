//
// Created by os on 8/13/23.
//

#include "../h/_Semaphore.hpp"
#include "../h/MemoryAllocator.hpp"


typedef _Semaphore _sem;
typedef _sem* sem_t;

int _Semaphore::sem_open(sem_t* handle, unsigned init){
    _Semaphore* s=(_Semaphore*)MemoryAllocator::_mem_alloc((sizeof (_Semaphore))/MEM_BLOCK_SIZE + ((sizeof (_Semaphore))%MEM_BLOCK_SIZE ? 1:0 ));
    if(s== nullptr)return -1;
    s->value=(int)init;
    s->active= true;
    s->blocked=ThreadList();
    *handle=s;
    return 0;
}

int _Semaphore::sem_close() {
    this->active= false;
    while(this->blocked.peekFirst()){
        TCB* thread = this->blocked.removeFirst();
        thread->setReady();
        Scheduler::put(thread);
    }
    return 0;
}

int  _Semaphore::wait(){
    if(!this->active)return -1;
    value--;
    if(value<0){
        this->blocked.addLast(TCB::running);
        TCB::running->setBlocked();
        TCB::dispatch();
    }

    return 0;
}


int _Semaphore::signal() {
    if(!this->active)return -1;
    value++;
    if(value<=0){
        TCB* thread = this->blocked.removeFirst();
        thread->setReady();
        Scheduler::put(thread);
    }

    return 0;
}
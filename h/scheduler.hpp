//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP

#include "../h/list.hpp"
#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"

class TCB;

class Scheduler{
private:
    static List<TCB> readyThreadQueue;

    static void *operator new(size_t n)
    {
        return MemoryAllocator::_mem_alloc(n/MEM_BLOCK_SIZE + (n% MEM_BLOCK_SIZE>0 ? 1:0));
    }

    static void operator delete(void *p) noexcept
    {
        MemoryAllocator::_mem_free(p);
    }
public:
    static TCB *get();

    static void put(TCB *tcb);

};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP

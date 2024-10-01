//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "MemoryAllocator.hpp"
//#include "../h/_Semaphore.hpp"

class _Semaphore;
// Thread Control Block
class TCB
{
public:
    typedef  TCB  _thread ;
    typedef _thread* thread_t;

    enum State{CREATED, READY, RUNNING, BLOCKED, FINISHED};

    ~TCB() { delete[] stack; }

    bool isFinished() const { return state==FINISHED; }

    bool isBlocked() const { return state==BLOCKED; }

    void setFinished() { state=FINISHED; }

    void setReady() { state=READY; }

    void setBlocked() { state=BLOCKED; }

    uint64 getTimeSlice() const { return timeSlice; }

    uint64 getId() const {return id ;}

    using Body = void (*)(void *);




    static int thread_create (
            thread_t* handle,
            void(*start_routine)(void*),
            void* arg,
            void* stack_space
    );

    static void threadExit();

    static void yield();

    static TCB *running;

    static int exitThread();

    static void dispatch();

    static void join(thread_t handle);

    void wait();


private:



    static uint64 ID;

    TCB(Body body, uint64 timeSlice) :
            state(CREATED),
            body(body),

            stack(body != nullptr ? (uint64 *)MemoryAllocator::_mem_alloc(DEFAULT_STACK_SIZE/MEM_BLOCK_SIZE + (DEFAULT_STACK_SIZE % MEM_BLOCK_SIZE>0 ? 1:0)): nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE-1] : 0
                    }),
            timeSlice(timeSlice),

            finished(false)

    {

        id=ID;
        if (body != nullptr) { Scheduler::put(this); }
    }

    TCB(Body body,void * stek,void *argumet, uint64 timeSlice) :
            state(CREATED),
            body(body),
            arg(argumet),
            stack(body != nullptr ? (uint64 *)stek: nullptr),

            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE-1] : 0
                    }),

            timeSlice(timeSlice),


            finished(false)

    {

        id=ID++;
        if (body != nullptr) { Scheduler::put(this); }
    }




    struct Context
    {
        uint64 ra;
        uint64 sp;
        //uint64 sepc;
    };
    uint64 id;
    State state;
    Body body;
    void * arg;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    _Semaphore* semafor;

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);



    static uint64 timeSliceCounter;


};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

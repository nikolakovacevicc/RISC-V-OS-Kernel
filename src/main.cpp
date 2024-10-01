#include "../h/tcb.hpp"
#include "../h/printing.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"
#include "../lib/console.h"

extern "C" void interrupt();
_sem *semafor;

static volatile bool finishedA = false;
static volatile bool finishedB = false;


void funkcija1(void * )
{
    //__putc('a');
    //sem_wait(semafor);
    for (uint64 i = 0; i < 5; i++) {
        //printInteger(2 * i);

        __putc('a');

        thread_dispatch();
    }
    finishedA = true;
    //sem_signal(semafor);
}

void funkcija2(void *)
{

    //sem_wait(semafor);
    for (uint64 i = 0; i < 5; i++) {
        //printInteger(3 * i);

        __putc('b');

        //__putc('\n');
        thread_dispatch();
    }
    finishedB = true;
    //sem_signal(semafor);
}



extern void userMain();

void wraper(void*){
    userMain();
}

void main()
{
    __asm__ volatile("mv t0, %0" : : "r" (&interrupt));
    __asm__ volatile("addi t0, t0,1");
    __asm__ volatile ("csrw stvec, t0");
    //Riscv::w_stvec((uint64) &interrupt);


    MemoryAllocator::_mem_init();
    //---------------------
    _thread *nit1;

    thread_create(&nit1,wraper, nullptr);

    _thread *main;
    thread_create(&main, nullptr, nullptr);

    TCB::running=main;
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
//------------------------------------------------------------
    /*while(true){
        //__putc('a');
        char c= __getc();
        __putc(c);
        if(c=='\n')break;
    }*/
   /* __putc('a');
    char c= __getc();
    __putc(c);*/


//**********************************
    //thread_join(nit1);
    TCB::running->join(nit1);
//**********************************
    /*void *a=mem_alloc(156);
    void *b=mem_alloc(156);

    *(uint64*)a=0xffffffffffffffff;
    *(uint64*)b=0xffffffffffffffff;

    MemoryAllocator::_mem_show();
    int x=mem_free(a);
    int y=mem_free(b);

    printInteger(x);
    printInteger(y);*/

    /*void * a= mem_alloc(sizeof (uint64));
    *(uint64*)a=0;*/


/*
    _thread *nit1;

    thread_create(&nit1,funkcija1, nullptr);



    _thread *nit2;

    thread_create(&nit2,funkcija2, nullptr);

    _thread *main;
    thread_create(&main, nullptr, nullptr);

    TCB::running=main;


    sem_open(&semafor,1);*/

    //thread_create(nullptr,main, nullptr);
    /*
    while (!nit1->isFinished() || !nit2->isFinished())
    {

        thread_dispatch();
        //sem_wait(semafor);
        __putc('#');
        //TCB::running->setBlocked();
    }*/
/*
    thread_join(nit1);
    thread_join(nit2);

*/

    //mem_free(a);


   //void *a=MemoryAllocator::_mem_alloc(4);

/*
    Thread* nit1 = new WorkerA();

    Thread* nit2 = new WorkerB();
    __putc('@');
    nit1->start();
    nit2->start();

    _thread *main;
    thread_create(&main, nullptr, nullptr);

    TCB::running=main;

    while (!(finishedA && finishedB )) {
        Thread::dispatch();
    }


*/

   //return 0;
}

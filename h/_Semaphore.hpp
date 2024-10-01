//
// Created by os on 8/13/23.
//

#ifndef PROJECT_BASE_V1_1__SEMAPHORE_HPP
#define PROJECT_BASE_V1_1__SEMAPHORE_HPP

#include "ThreadList.hpp"
#include "tcb.hpp"

class _Semaphore {
public :

    typedef _Semaphore _sem;
    typedef _sem* sem_t;

    _Semaphore(uint64 init=1):value(init),active(true){};
    ~_Semaphore();

    static int sem_open(sem_t* handle, unsigned init);
    int sem_close();
    int  wait();
    int  signal();

    int getValue()const{return value;}

private:
    int value;
    ThreadList blocked;
    bool active;

    friend class Riscv;

};


#endif //PROJECT_BASE_V1_1__SEMAPHORE_HPP

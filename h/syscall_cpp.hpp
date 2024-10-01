//
// Created by os on 8/20/23.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_1_SYSCALL_CPP_HPP


#include "syscall_c.hpp"
#include "list.hpp"
void* operator new (size_t);
void operator delete (void*);
void *operator new[](size_t n);
void operator delete[](void *p);


class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    void join();
    static void dispatch ();
    static int sleep (time_t);
    static void threadWrapper(void *);
    static void waitForAll();
protected:
    Thread ();
    virtual void run (){};
private:
    static List<Thread> lista;
    thread_t myHandle;
    void (*body)(void*); void* arg;
};



class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};


#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_HPP

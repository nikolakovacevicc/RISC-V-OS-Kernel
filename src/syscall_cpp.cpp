//
// Created by os on 8/20/23.
//

#include "../h/syscall_cpp.hpp"

void *operator new(size_t size) {
    return mem_alloc(size);
}

void operator delete(void *adr) {
    mem_free(adr);
}

void *operator new[] (size_t size) {

    return mem_alloc(size);
}

void operator delete[] (void *p) {
    mem_free(p);
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->body = body;
    this->arg = arg;

}

Thread::~Thread() {
    //thread_exit();
    //delete myHandle;
}

int Thread::start() {
    if (body != nullptr) {
        return thread_create(&myHandle, body, arg);
    } else {

        /*return thread_create(&myHandle,[](void* ptr) {
            static_cast<Thread*>(ptr)->run();
        },arg);*/
        return thread_create(&myHandle, Thread::threadWrapper, (void*)this);
        //return 0;
        //return thread_create(&myHandle, Thread::threadWrapper, (void *) this);
    }

}

void Thread::threadWrapper(void *thread) {

    ((Thread *) thread)->run();
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread() {
    this->body = nullptr;

    this->arg = nullptr;


}

void Thread::join() {
    thread_join(myHandle);
}

int Thread::sleep(time_t) {
    return 0;
}

void Thread::waitForAll() {

}


Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

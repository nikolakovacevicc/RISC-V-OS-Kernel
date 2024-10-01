//
// Created by os on 8/19/23.
//

#ifndef PROJECT_BASE_V1_1_THREADLIST_HPP
#define PROJECT_BASE_V1_1_THREADLIST_HPP

#include "../lib/mem.h"
#include "MemoryAllocator.hpp"
#include "../h/tcb.hpp"


class ThreadList
{
private:
    struct Elem
    {
        TCB *data;
        Elem *next;

        Elem(TCB *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head, *tail;




public:
    ThreadList() : head(nullptr), tail(nullptr) {}

    ThreadList(const ThreadList &) = delete;

    //ThreadList &operator=(const ThreadList &) = delete;



    void addLast(TCB *data)
    {
        //Elem *elem = new Elem(data, 0);
        int s=sizeof (Elem);
        Elem *elem =(Elem*)MemoryAllocator::_mem_alloc(s/MEM_BLOCK_SIZE+(s%MEM_BLOCK_SIZE ? 1:0));
        elem->data=data;
        elem->next=nullptr;
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    TCB *removeFirst()
    {
        if (!head) { return nullptr; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = nullptr; }

        TCB *ret = elem->data;
        //delete elem;
        MemoryAllocator::_mem_free(elem);
        return ret;

    }

    TCB *peekFirst()
    {
        if (!head) { return nullptr; }
        return head->data;
    }


};


#endif //PROJECT_BASE_V1_1_THREADLIST_HPP

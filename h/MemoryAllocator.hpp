#ifndef MemoryAllocator_hpp
#define MemoryAllocator_hpp

#include "../lib/hw.h"
#include "../lib/console.h"

struct MemBlock{
    void * start;
    size_t size;
    bool isUsed;
};

class MemoryAllocator{
public:
    static void* _mem_alloc(size_t size);
    static int _mem_free(void* ptr);
    static void _mem_init();
    static void _mem_show();


private:


    static struct MemBlock* arr;
    static size_t numElements;

    static int findFreeBlock(size_t size);


};

#endif //MemoryAllocator_hpp
#include "../h/MemoryAllocator.hpp"
MemBlock* MemoryAllocator::arr =(MemBlock*)HEAP_START_ADDR;
size_t MemoryAllocator::numElements = 0;

void MemoryAllocator::_mem_init() {
    arr =(MemBlock*)HEAP_START_ADDR;
    numElements = 0;
    size_t heapSize = reinterpret_cast<const char*>(HEAP_END_ADDR) - reinterpret_cast<const char*>(HEAP_START_ADDR);
    numElements = heapSize / MEM_BLOCK_SIZE;

    for(size_t i=0;i<numElements;i++){

        arr[i].start = static_cast<void*>(static_cast<char*>(const_cast<void*>(HEAP_START_ADDR))+ numElements*sizeof(MemBlock)+ i * MEM_BLOCK_SIZE);
        arr[i].size=0;
        arr[i].isUsed=false;
    }

}

int MemoryAllocator::findFreeBlock(size_t size) {
    for (size_t i = 0; i < numElements; i++) {
        if (!arr[i].isUsed) {
            bool flag=false;
            for(size_t j=i+1;j<i+size;j++){
                if (j >= numElements || arr[j].isUsed) {
                    flag = true;
                    break;
                }
            }

            if (!flag) {
                return i;
            }

        }
    }

    return -1;
}

void* MemoryAllocator::_mem_alloc(size_t size) {
    int blockStart = findFreeBlock(size);
    if (blockStart!=-1) {
        for(size_t i=blockStart;i<blockStart+size;i++){
            arr[i].isUsed=true;
            arr[i].size=size-(i-blockStart);
        }
        return arr[blockStart].start;
    }
    return nullptr;
}


int MemoryAllocator::_mem_free(void* ptr) {
    if (ptr == nullptr) {
        return -1;
    }

    if (ptr < HEAP_START_ADDR || ptr >= HEAP_END_ADDR) {
        return -2;
    }


    uint64 blockIndex = static_cast<size_t>((static_cast<char*>(ptr) - (static_cast<char*>(const_cast<void*>(HEAP_START_ADDR))+ numElements*sizeof (MemBlock))) / MEM_BLOCK_SIZE);



    if (blockIndex >= numElements) {
        return -2;
    }

    if (!arr[blockIndex].isUsed) {

        return -3;
    }


    size_t size = arr[blockIndex].size;
    for(size_t i=0;i<size;i++){
        arr[blockIndex+i].isUsed= false;
        arr[blockIndex+i].size=0;
    }

    return 0;
}

void MemoryAllocator::_mem_show() {
    for(size_t i=0;i<10;i++){

        //printInt((uint64)arr[i].start);
        __putc('\n');

        __putc(arr[i].isUsed ? '1':'0' );
        __putc('\n');
        //printInt(arr[i].size);
        __putc('\n');
        __putc('\n');
    }
}

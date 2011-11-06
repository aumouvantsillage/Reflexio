
#ifndef __RX_CORE_MEMORY_H__
#define __RX_CORE_MEMORY_H__

#include <stdio.h>
#include <stdlib.h>

#ifndef RX_DEBUG_MEMORY

#define RXMemory_allocate(size) malloc(size)

#define RXMemory_deallocate(ptr, size) free(ptr)

#define RXMemory_printStats \
    printf("Recompile with flag RX_DEBUG_MEMORY.\n")

#else

extern size_t RXMemory_maxAllocatedBytes;
extern size_t RXMemory_currentAllocatedBytes;

#define RXMemory_allocate(size) ({ \
    RXMemory_currentAllocatedBytes += size; \
    if (RXMemory_currentAllocatedBytes > RXMemory_maxAllocatedBytes) { \
        RXMemory_maxAllocatedBytes = RXMemory_currentAllocatedBytes; \
    } \
    malloc(size); \
})

#define RXMemory_deallocate(ptr, size) \
    free(ptr); \
    RXMemory_currentAllocatedBytes -= size

#define RXMemory_printStats \
    printf("Max allocated memory: %zu\nNon-freed memory: %zu\n", \
        RXMemory_maxAllocatedBytes, RXMemory_currentAllocatedBytes)

#endif


#endif


#ifndef __RX_CORE_MEMORY_H__
#define __RX_CORE_MEMORY_H__

#include <stdlib.h>

#ifndef RX_DEBUG_MEMORY

#define RXMemory_allocate(size) malloc(size)

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

#endif


#endif

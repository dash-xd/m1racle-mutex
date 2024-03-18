#include <stdio.h>
#include "mutex.h"

int main() {
    // Example usage of custom locks
    miReaderLock();
    printf("Reader locked\n");
    miReaderUnlock();
    printf("Reader unlocked\n");

    miWriterLock();
    printf("Writer locked\n");
    miWriterUnlock();
    printf("Writer unlocked\n");

    // Example usage of barrier synchronization
    miBarrierInit(5);
    for (int i = 0; i < 5; i++) {
        miBarrier();
        printf("Barrier reached by thread %d\n", i);
    }

    // Example usage of semaphore
    miSemaphore(3);
    for (int i = 0; i < 5; i++) {
        miSemaphoreWait();
        printf("Semaphore acquired by thread %d\n", i);
    }

    // Example usage of event handling
    miEventWait();
    printf("Event waited\n");
    miEventSignal();
    printf("Event signaled\n");

    // Example usage of transactional memory
    miTransactionalMemory();
    printf("Transactional memory operation completed\n");

    // Example usage of distributed coordination
    miDistributedCoordination();
    printf("Distributed lock acquired\n");
    miReleaseDistributedLock();
    printf("Distributed lock released\n");

    return 0;
}

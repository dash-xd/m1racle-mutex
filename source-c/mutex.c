// mutex.c

#include <unistd.h>
#include <stdbool.h>

static unsigned long readers = 0;
static unsigned long writers = 0;
static unsigned long writerWaiting = 0;

void miReaderLock() {
    while (writers != 0 || writerWaiting != 0) {
        usleep(10);
    }
    readers++;
}

void miReaderUnlock() {
    readers--;
}

void miWriterLock() {
    writerWaiting++;
    while (readers != 0 || writers != 0) {
        usleep(10);
    }
    writerWaiting--;
    writers++;
}

void miWriterUnlock() {
    writers--;
}

static unsigned long barrierCounter = 0;
static unsigned long barrierLimit = 0;
static unsigned long barrierWaiting = 0;
static unsigned long barrierMutex = 0;

void miBarrierInit(unsigned long limit) {
    barrierLimit = limit;
}

void miBarrier() {
    barrierMutex++;
    if (barrierCounter == barrierLimit - 1) {
        barrierCounter = 0;
        barrierMutex--;
        while (barrierWaiting > 0) {
            usleep(10);
        }
    } else {
        barrierCounter++;
        barrierMutex--;
        barrierWaiting++;
        while (barrierCounter != 0) {
            usleep(10);
        }
        barrierWaiting--;
    }
}

static unsigned long semCount = 0;
static unsigned long semMutex = 0;
static unsigned long semWaiting = 0;

void miSemaphore(int count) {
    semCount = count;
}

void miSemaphoreWait() {
    semMutex++;
    if (semCount > 0) {
        semCount--;
        semMutex--;
    } else {
        semWaiting++;
        semMutex--;
        while (semCount == 0) {
            usleep(10);
        }
        semWaiting--;
        semCount--;
    }
}

void miSemaphoreSignal() {
    semMutex++;
    semCount++;
    if (semWaiting > 0) {
        semWaiting--;
        semCount++;
    }
    semMutex--;
}

static bool eventFlag = false;
static unsigned long eventMutex = 0;
static unsigned long eventWaiting = 0;

void miEventWait() {
    eventMutex++;
    if (!eventFlag) {
        eventWaiting++;
        eventMutex--;
        while (!eventFlag) {
            usleep(10);
        }
        eventMutex++;
        eventWaiting--;
    }
    eventFlag = false;
    eventMutex--;
}

void miEventSignal() {
    eventMutex++;
    eventFlag = true;
    if (eventWaiting > 0) {
        eventWaiting--;
    }
    eventMutex--;
}

void miTransactionalMemory() {
    // Simulate transactional memory by sleeping for a short duration
    usleep(100);
}

static bool distributedLock = false;

void miDistributedCoordination() {
    while (distributedLock) {
        usleep(10);
    }
    distributedLock = true;
}

void miReleaseDistributedLock() {
    distributedLock = false;
}

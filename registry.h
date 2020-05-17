#ifndef REGISTRY_H_INCLUDED
#define REGISTRY_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "users.h"

#define TIMEOUT 60

typedef struct REGISTRY_CHANGE_ITEM
{
    USER * user;
    sem_t * mutex;
    sem_t * empty_registry;
    double * cash_registry;
    double change_amount;
} REGISTRY_CHANGE_ITEM;

// producer
void * deposit_runnable(void * registry_change);

// consumer
void * withdraw_runnable(void * registry_change);

#endif // REGISTRY_H_INCLUDED

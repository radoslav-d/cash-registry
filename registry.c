#include "registry.h"

// producer
void * deposit_runnable(void * void_registry_change)
{
    REGISTRY_CHANGE_ITEM * registry_change = (REGISTRY_CHANGE_ITEM *) void_registry_change;

    sem_wait(registry_change->mutex);

    (*registry_change->cash_registry) += registry_change->change_amount;
    registry_change->user->balance -= registry_change->change_amount;
    sleep(TIMEOUT);

    sem_post(registry_change->mutex);
    sem_post(registry_change->empty_registry);

    return NULL;
}

// consumer
void * withdraw_runnable(void * void_registry_change)
{
    REGISTRY_CHANGE_ITEM * registry_change = (REGISTRY_CHANGE_ITEM *) void_registry_change;

    sem_wait(registry_change->mutex);

    if (*registry_change->cash_registry <= 0)
    {
        sem_wait(registry_change->empty_registry);
    }
    (*registry_change->cash_registry) -= registry_change->change_amount;
    registry_change->user->balance += registry_change->change_amount;
    sleep(TIMEOUT);

    sem_post(registry_change->mutex);

    return NULL;
}

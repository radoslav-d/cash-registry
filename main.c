#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include "registry.h"
#include "users.h"

void init();
void register_new_user();
void remove_user();
void deposit();
void withdraw();
void cash_registry_action(char * action_name, void *(*runnable) (void *));

double CASH_REGISTRY = 0;
sem_t MUTEX;
sem_t EMPTY_CASH_REGISTRY;
USER_LIST_ITEM * ROOT_USER_ITEM;

int main()
{
    printf("Hello!\n");
    init();
    int user_option;
    while (1)
    {
        printf("Available options:\n");
        printf("1. Register new user\n");
        printf("2. Remove user\n");
        printf("3. Deposit in the cash registry\n");
        printf("4. Withdraw from the cash registry\n");
        printf("0. Exit\n");
        printf("You option: ");
        scanf("%d", &user_option);
        switch (user_option)
        {
            case 1:
                register_new_user();
                break;
            case 2:
                remove_user();
                break;
            case 3:
                deposit();
                break;
            case 4:
                withdraw();
                break;
            case 0:
                printf("Goodbye!\n");
                persist_users(ROOT_USER_ITEM);
                free_users_memory(ROOT_USER_ITEM);
                return 0;
            default:
                printf("Invalid option!\n");
        }

    }
    return 0;
}

void init()
{
    sem_init(&EMPTY_CASH_REGISTRY, 0, 0);
    sem_init(&MUTEX, 0, 1);
    ROOT_USER_ITEM = load_users();
}

void register_new_user()
{
    USER user;
    user.balance = 0;
    user.id = generate_unique_id(ROOT_USER_ITEM);
    printf("Generated ID is %d\n", user.id);
    printf("Enter name: ");
    scanf("%s", user.name);
    ROOT_USER_ITEM = add_user(ROOT_USER_ITEM, user);
}

void remove_user()
{
    printf("Enter user ID: ");
    int id;
    scanf("%d", &id);
    ROOT_USER_ITEM = delete_user(ROOT_USER_ITEM, id);
}

void deposit()
{
    cash_registry_action("deposit", deposit_runnable);
}

void withdraw()
{
    cash_registry_action("withdraw", withdraw_runnable);
}

void cash_registry_action(char * action_name, void *(*runnable) (void *))
{
    char name[MAX_USERNAME_LEN];
    printf("Enter you name: ");
    scanf("%s", name);

    REGISTRY_CHANGE_ITEM * change_item = (REGISTRY_CHANGE_ITEM *) malloc(sizeof(REGISTRY_CHANGE_ITEM));

    USER * user_ptr = get_user_by_name(ROOT_USER_ITEM, name);
    if (user_ptr == NULL)
    {
        printf("No user with such name!\n");
        return;
    }
    change_item->user = user_ptr;
    change_item->cash_registry = &CASH_REGISTRY;
    change_item->mutex = &MUTEX;
    change_item->empty_registry = &EMPTY_CASH_REGISTRY;
    printf("Enter how much you want to %s: ", action_name);
    double amount;
    scanf("%lf", &amount);

    // abs
    amount = amount >= 0 ? amount : amount * -1;

    change_item->change_amount = amount;

    pthread_t thread;
    pthread_create(&thread, NULL, runnable,(void*) change_item);
}

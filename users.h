#ifndef USERS_H_INCLUDED
#define USERS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILENAME ".userdata"

typedef struct
{
    int id;
    char name[50];
    double balance;
} USER;

typedef struct USER_LIST_ITEM
{
    USER user;
    struct USER_LIST_ITEM * next;
} USER_LIST_ITEM;

USER * get_user_by_name(USER_LIST_ITEM * root_item, char * name);
int generate_unique_id(USER_LIST_ITEM * root_item);
USER_LIST_ITEM * add_user(USER_LIST_ITEM * root_item, USER user);
USER_LIST_ITEM * delete_user(USER_LIST_ITEM * root_item, int id);
void persist_users(USER_LIST_ITEM * root_item);
void free_users_memory(USER_LIST_ITEM * root_item);
USER_LIST_ITEM * load_users();


#endif // USERS_H_INCLUDED

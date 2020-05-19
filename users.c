#include "users.h"

USER_LIST_ITEM * pop_user(USER_LIST_ITEM * root_item);

int generate_unique_id(USER_LIST_ITEM * root_item)
{
    if (root_item == NULL)
    {
        return 0;
    }
    long max_id = -1;
    USER_LIST_ITEM * current_item;
    for (current_item = root_item; current_item != NULL; current_item = current_item->next)
    {
        if (current_item->user.id > max_id)
        {
            max_id = current_item->user.id;
        }
    }
    return max_id + 1;
}

USER * get_user_by_name(USER_LIST_ITEM * root_item, char * name)
{
    USER_LIST_ITEM * current_item;
    for (current_item = root_item; current_item != NULL; current_item = current_item->next)
    {
        if (strcmp(current_item->user.name, name) == 0)
        {
            return &(current_item->user);
        }
    }
    return NULL;
}


USER_LIST_ITEM * add_user(USER_LIST_ITEM * root_item, USER user)
{
    USER_LIST_ITEM * user_item = (USER_LIST_ITEM *) malloc(sizeof(USER_LIST_ITEM));

    if (user_item == NULL)
    {
        fprintf(stderr, "Memory allocation error!\n");
        exit(1);
    }
    user_item->user = user;
    user_item->next = root_item;

    return user_item;
}

USER_LIST_ITEM * delete_user(USER_LIST_ITEM * root_item, int id)
{
    USER_LIST_ITEM * current_item = root_item;
    USER_LIST_ITEM * previous_item = root_item;

    while (current_item != NULL)
    {
        if (current_item->user.id == id)
        {
            if (current_item == root_item)
            {
                return pop_user(root_item);
            }
            previous_item->next = current_item->next;
            free(current_item);
            return root_item;
        }
        previous_item = current_item;
        current_item = current_item->next;
    }
    fprintf(stderr, "No user deleted! ID does not match!\n");
    return root_item;
}

USER_LIST_ITEM * pop_user(USER_LIST_ITEM * root_item)
{
    USER_LIST_ITEM * new_root_item = root_item->next;
    free(root_item);
    return new_root_item;
}

void persist_users(USER_LIST_ITEM * root_item)
{
    int file_descriptor = -1;
    file_descriptor = open(FILENAME, O_CREAT | O_WRONLY);

    if (file_descriptor == -1)
    {
        fprintf(stderr, "Error opening file for persistence!\n");
        return;
    }

    USER_LIST_ITEM * current_item;
    for (current_item = root_item; current_item != NULL; current_item = current_item->next)
    {
        if (write(file_descriptor, &current_item->user, sizeof(USER)) == -1)
        {
            fprintf(stderr, "Error when persisting users!\n");
            return;
        }
    }
    close(file_descriptor);
}

USER_LIST_ITEM * load_users()
{
    int file_descriptor = -1;
    file_descriptor = open(FILENAME, O_RDONLY);

    if (file_descriptor == -1)
    {
        fprintf(stderr, "Cannot open persistence file!\n");
        return NULL;
    }

    USER_LIST_ITEM * root_item = NULL;
    USER user;

    while(read(file_descriptor, &user, sizeof(USER)) == sizeof(USER))
    {
        root_item = add_user(root_item, user);
    }
    close(file_descriptor);
    return root_item;
}

void free_users_memory(USER_LIST_ITEM * root_item)
{
    USER_LIST_ITEM * current_item;
    while (root_item != NULL)
    {
        current_item = root_item;
        root_item = root_item->next;
        free(current_item);
    }
}

#pragma once

struct msg_struct
{
    char* data;
    int len;
    struct msg_struct* next;
};

struct msg_list
{
    struct msg_struct* head;
    struct msg_struct* tail;
};

struct msg_list* list_add_element(struct msg_list*, const char* data, const int);
struct msg_list* list_remove_element(struct msg_list*);

struct msg_list* list_new(void);
struct msg_list* list_free( struct msg_list*);

void list_print(const struct msg_list*);
void list_print_element(const struct msg_struct*);


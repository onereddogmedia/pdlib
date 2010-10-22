/* This Queue implementation of singly linked list in C implements 3
 * operations: add, remove and print elements in the list.  Well, actually,
 * it implements 4 operations, lats one is list_free() but free() should not
 * be considered the operation but  a mandatory practice like brushing
 * teeth every morning, otherwise you will end up loosing some part of
 * your body(the software) Its is the modified version of my singly linked
 * list suggested by Ben from comp.lang.c . I was using one struct to do
 * all the operations but Ben added a 2nd struct to make things easier and
 * efficient.
 *
 * I was always using the strategy of searching through the list to find the
 *  end and then addd the value there. That way list_add() was O(n). Now I
 * am keeping track of tail and always use  tail to add to the linked list, so
 * the addition is always O(1), only at the cost of one assignment.
 *
 *
 * VERISON 0.5
 *
 */

#include <queue.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 0

int main(void)
{
    struct msg_list* mt = NULL;

    mt = list_new();
    list_add_element(mt, 1);
    list_add_element(mt, 2);
    list_add_element(mt, 3);
    list_add_element(mt, 4); 

    list_print(mt);

    list_remove_element(mt);
    list_print(mt);

    list_free(mt);   /* always remember to free() the malloc()ed memory */
    free(mt);        /* free() if list is kept separate from free()ing the structure, I think its a good design */
    mt = NULL;      /* after free() always set that pointer to NULL, C will run havon on you if you try to use a dangling pointer */

    list_print(mt);

    return 0;
}

#endif

/* Will always return the pointer to msg_list */
struct msg_list* list_add_element(struct msg_list* s, const char* msg, const int len)
{
    if (NULL == s)
    {
        fprintf(stderr, "IN list_add: s null\n");
        return s;
    }
    if (NULL == msg)
    {
        fprintf(stderr, "IN list_add: msg null\n");
        return s;
    }
    if (0 == len)
    {
        fprintf(stderr, "IN list_add: len is 0\n");
        return s;
    }

    struct msg_struct* p = malloc(1 * sizeof(*p));

    if (NULL == p)
    {
        fprintf(stderr, "IN list_add: malloc() failed\n");
        return s;
    }

    p->data = malloc(1 * len);
    if (NULL == p->data)
    {
        fprintf(stderr, "IN list_add malloc() 2 failed\n");
        return s;
    }
    
    p->len = len;
    memcpy(p->data, msg, len);
    p->next = NULL;

    if (NULL == s)
    {
        printf("Queue not initialized\n");
        return s;
    }
    else if (NULL == s->head && NULL == s->tail)
    {
        /* printf("Empty list, adding p->num: %d\n\n", p->num);  */
        s->head = s->tail = p;
        return s;
    }
    else if (NULL == s->head || NULL == s->tail )
    {
        fprintf(stderr, "There is something seriously wrong with your assignment of head/tail to the list\n");
        free(p);
        return NULL;
    }
    else
    {
        /* printf("List not empty, adding element to tail\n"); */
        s->tail->next = p;
        s->tail = p;
    }

    return s;
}

/* This is a queue and it is FIFO, so we will always remove the first element */
struct msg_list* list_remove_element(struct msg_list* s)
{
    struct msg_struct* h = NULL;
    struct msg_struct* p = NULL;

    if (NULL == s)
    {
        printf("List is empty\n");
        return s;
    }
    else if (NULL == s->head && NULL == s->tail)
    {
        printf("Well, List is empty\n");
        return s;
    }
    else if (NULL == s->head || NULL == s->tail)
    {
        printf("There is something seriously wrong with your list\n");
        printf("One of the head/tail is empty while other is not \n");
        return s;
    }

    h = s->head;
    p = h->next;
    free(h->data);
    free(h);
    s->head = p;
    if (NULL == s->head) s->tail = s->head;   /* The element tail was pointing to is free(), so we need an update */

    return s;
}

/* ---------------------- small helper fucntions ---------------------------------- */
struct msg_list* list_free(struct msg_list* s)
{
    while (s->head)
    {
        list_remove_element(s);
    }

    return s;
}

struct msg_list* list_new(void)
{
    struct msg_list* p = malloc(1 * sizeof(*p));

    if (NULL == p)
    {
        fprintf(stderr, "LINE: %d, malloc() failed\n", __LINE__);
    }
    else
    {
        p->head = p->tail = NULL;
    }

    return p;
}

void list_print(const struct msg_list* ps)
{
    struct msg_struct* p = NULL;

    if (ps)
    {
        for (p = ps->head; p; p = p->next)
        {
            list_print_element(p);
        }
    }

    printf("------------------\n");
}

void list_print_element(const struct msg_struct* p)
{
    if (p)
    {
        printf("Data = %s [%d]\n", p->data, p->len);
    }
    else
    {
        printf("Can not print NULL struct \n");
    }
}

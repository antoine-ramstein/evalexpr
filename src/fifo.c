#include "fifo.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct fifo *fifo_init(void)
{
    struct fifo *fifo = malloc(sizeof(struct fifo));
    if (!fifo)
    {
        return NULL;
    }
    fifo->head = NULL;
    fifo->tail = NULL;
    fifo->size = 0;
    return fifo;
}

size_t fifo_size(struct fifo *fifo)
{
    return fifo->size;
}

void fifo_push(struct fifo *fifo, struct elem *e)
{
    struct list *list = malloc(sizeof(struct list));
    if (!list)
    {
        return;
    }
    list->data = e;
    list->next = NULL;
    if (fifo->tail)
    {
        fifo->tail->next = list;
    }
    else
    {
        fifo->head = list;
    }
    fifo->tail = list;
    fifo->size += 1;
}

struct elem *fifo_head(struct fifo *fifo)
{
    return fifo->head->data;
}

void fifo_pop(struct fifo *fifo)
{
    if (!fifo || fifo->size == 0)
    {
        return;
    }
    struct list *list = fifo->head;
    fifo->head = fifo->head->next;
    if (!fifo->head)
    {
        fifo->tail = NULL;
    }
    fifo->size -= 1;
    free(list);
}

/*void fifo_print(const struct fifo *fifo)
{
    if (!fifo || fifo->size == 0)
    {
        return;
    }
    struct list *list = fifo->head;
    while (list != NULL)
    {
        printf("%d\n", list->data);
        list = list->next;
    }
}*/

void fifo_clear(struct fifo *fifo)
{
    if (!fifo)
    {
        return;
    }
    struct list *f = fifo->head;
    while (f)
    {
        struct list *next = f->next;
        free(f);
        f = next;
    }
    fifo->head = NULL;
    fifo->tail = NULL;
    fifo->size = 0;
}

void fifo_destroy(struct fifo *fifo)
{
    if (!fifo)
    {
        return;
    }
    fifo_clear(fifo);
    free(fifo);
}

/*int main(void){
    struct fifo *f = fifo_init();
    fifo_push(f,3);
    fifo_push(f,-2);
    fifo_push(f,1);
    fifo_print(f);
    printf("\n");
    fifo_pop(f);
    fifo_print(f);
    printf("\n");
    fifo_clear(f);
    fifo_push(f,3);
    fifo_print(f);
    fifo_destroy(f);
    return 0;
}*/

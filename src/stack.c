#include "stack.h"

#include <stddef.h>
// #include <stdio.h>
#include <stdlib.h>

struct stack *stack_push(struct stack *s, struct elem *e)
{
    if (!s)
    {
        struct stack *res = malloc(sizeof(struct stack));
        if (!res)
        {
            return NULL;
        }
        res->data = e;
        res->next = NULL;
        return res;
    }
    struct stack *res = malloc(sizeof(struct stack));
    if (!res)
    {
        return NULL;
    }
    res->data = e;
    res->next = s;
    return res;
}

struct stack *stack_pop(struct stack *s)
{
    if (!s)
    {
        return NULL;
    }
    struct stack *next = s->next;
    free(s);
    return next;
}

struct elem *stack_peek(struct stack *s)
{
    return s->data;
}

/*int main(void)
{
    struct stack *s = malloc(sizeof(struct stack));
    s->data = 4;
    s->next = NULL;
    printf("top of the stack %d\n", stack_peek(s));
    s = stack_push(s, 3);
    printf("top of the stack %d\n", stack_peek(s));
    s = stack_push(s, 2);
    printf("top of the stack %d\n", stack_peek(s));
    s = stack_push(s, 1);
    printf("top of the stack %d\n", stack_peek(s));
    s = stack_pop(s);
    printf("top of the stack %d\n", stack_peek(s));
    return 0;
}*/

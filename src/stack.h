#ifndef STACK_H
#define STACK_H

#include "token.h"

struct stack
{
    struct elem *data;
    struct stack *next;
};

struct stack *stack_push(struct stack *s, struct elem *e);
struct stack *stack_pop(struct stack *s);
struct elem *stack_peek(struct stack *s);

#endif /* ! STACK_H */

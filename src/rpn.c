#define _POSIX_C_SOURCE 200809L

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "token.h"

static void free_tokens(struct elem *head)
{
    while (head)
    {
        struct elem *next = head->next;
        free(head);
        head = next;
    }
}

static void free_stack(struct stack *s)
{
    while (s)
    {
        struct stack *next = s->next;
        free(s);
        s = next;
    }
}

static int is_number(const char *s, int *out)
{
    if (!s || *s == '\0')
        return 0;

    int sign = 1;
    int i = 0;

    while (s[i] == '+' || s[i] == '-')
    {
        if (s[i] == '-')
            sign = -sign;
        i++;
    }

    if (s[i] < '0' || s[i] > '9')
        return 0;

    *out = atoi(&s[i]) * sign;
    return 1;
}

static struct elem *define(int data, enum Token token, struct elem *res)
{
    res->data = data;
    res->token = token;
    res->next = NULL;
    return res;
}

static struct elem *get_token(char *c)
{
    struct elem *res = malloc(sizeof(struct elem));
    int out;
    if (!res)
        return NULL;
    if (strcmp(c, "+") == 0)
        return define(-1, ADD, res);

    if (strcmp(c, "-") == 0)
        return define(-1, SUB, res);

    if (strcmp(c, "*") == 0)
        return define(-1, MULT, res);

    if (strcmp(c, "/") == 0)
        return define(-1, DIV, res);

    if (strcmp(c, "%") == 0)
        return define(-1, MODULO, res);

    if (strcmp(c, "^") == 0)
        return define(-1, POW, res);

    if (is_number(c, &out))
        return define(out, N, res);

    else
        return define(-1, NOTHING, res);
}

static struct elem *parse(const char *s, int *error)
{
    struct elem *head = NULL;
    struct elem *tail = NULL;
    char *input = strdup(s);
    if (!input)
    {
        *error = 4;
        return NULL;
    }
    char *token;
    char *rest = input;
    while ((token = strtok_r(rest, " ", &rest)))
    {
        struct elem *node = get_token(token);
        if (!node)
            continue;
        if (node->token == NOTHING)
        {
            *error = 1;
            free_tokens(node);
            free(input);
            return NULL;
        }
        if (!head)
            head = node;
        else
            tail->next = node;
        tail = node;
    }
    free(input);
    return head;
}

static int my_pow(int a, int b)
{
    if (b < 0)
        return 0;

    int res = 1;
    while (b > 0)
    {
        if (b % 2 == 1)
            res *= a;
        a *= a;
        b /= 2;
    }
    return res;
}

static int apply_op(struct elem *left, struct elem *right, enum Token t,
                    int *error)
{
    if (t == ADD)
        left->data += right->data;
    else if (t == SUB)
        left->data -= right->data;
    else if (t == MULT)
        left->data *= right->data;
    else if (t == DIV)
    {
        if (right->data == 0)
        {
            *error = 3;
            return 0;
        }
        left->data /= right->data;
    }
    else if (t == MODULO)
    {
        if (right->data == 0)
        {
            *error = 3;
            return 0;
        }
        left->data %= right->data;
    }
    else if (t == POW)
    {
        if (right->data < 0)
        {
            *error = 3;
            return 0;
        }
        left->data = my_pow(left->data, right->data);
    }
    else
    {
        *error = 2;
        return 0;
    }
    return 1;
}

int valuate(const char *str, int *error)
{
    *error = 0;
    if (!str || !*str)
        return 0;
    struct elem *elem = parse(str, error);
    if (!elem)
        return 0;
    struct stack *stack = NULL;
    struct elem *current = elem;
    while (current)
    {
        if (current->token == N)
        {
            stack = stack_push(stack, current);
        }
        else
        {
            if (!stack || !stack->next)
            {
                *error = 2;
                break;
            }
            struct elem *right = stack->data;
            stack = stack_pop(stack);
            struct elem *left = stack->data;
            stack = stack_pop(stack);
            if (!apply_op(left, right, current->token, error))
                break;
            stack = stack_push(stack, left);
        }
        current = current->next;
    }

    int result = (!*error && stack && !stack->next) ? stack->data->data : 0;
    if (!*error && (!stack || stack->next))
        *error = 2;

    free_stack(stack);
    free_tokens(elem);
    return result;
}

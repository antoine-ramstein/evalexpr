#define _POSIX_C_SOURCE 200809L

#include "stdn.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fifo.h"
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

static void rev(char *s)
{
    int l = 0;
    int r = strlen(s) - 1;
    char t;

    while (l < r)
    {
        t = s[l];
        s[l] = s[r];
        s[r] = t;

        l++;
        r--;
    }
}

char *my_itoa(int value, char *s)
{
    if (value == 0)
    {
        char *save = s;
        *s = '0';
        s++;
        *s = '\0';
        return save;
    }
    int neg = 0;
    if (value < 0)
    {
        neg = 1;
        value *= -1;
    }
    char *save = s;
    while (value != 0)
    {
        char digit = value % 10 + '0';
        *s = digit;
        value = value / 10;
        s++;
    }
    if (neg)
    {
        *s = '-';
        s++;
    }
    *s = '\0';
    rev(save);
    return save;
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
    if (strcmp(c, "(") == 0)
        return define(-1, L_PAR, res);
    if (strcmp(c, ")") == 0)
        return define(-1, R_PAR, res);
    if (is_number(c, &out))
        return define(out, N, res);

    else
    {
        fprintf(stderr, "Invalid Syntax\n");
        exit(1);
    }
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

static int is_operator(struct elem *elem)
{
    if (!elem)
    {
        return 0;
    }
    switch (elem->token)
    {
    case ADD:
    case SUB:
    case MULT:
    case DIV:
    case MODULO:
    case POW:
        return 1;
    default:
        return 0;
    }
}

static int is_greater_pred(enum Token e1, enum Token e2)
{
    if (e1 == ADD || e1 == SUB)
    {
        return 0;
    }
    if (e1 == MULT || e1 == DIV || e1 == MODULO)
    {
        if (e2 == ADD || e2 == SUB)
            return 1;
        return 0;
    }
    if (e1 == POW)
    {
        if (e2 == MULT || e2 == DIV || e2 == MODULO || e2 == ADD || e2 == SUB)
        {
            return 1;
        }
        return 0;
    }
    return 0;
}

static struct stack *push_operators_until_lower(struct stack *stack,
                                                struct elem *current,
                                                struct fifo *queue)
{
    while (stack && stack_peek(stack) && is_operator(stack_peek(stack))
           && is_greater_pred(stack_peek(stack)->token, current->token))
    {
        fifo_push(queue, stack_peek(stack));
        stack = stack_pop(stack);
    }
    return stack;
}

static struct stack *process_r_par(struct stack *stack, struct fifo *queue)
{
    while (stack_peek(stack)->token != L_PAR)
    {
        fifo_push(queue, stack_peek(stack));
        stack = stack_pop(stack);
        if (!stack)
        {
            fprintf(stderr, "Invalid Syntax\n");
            exit(2);
        }
    }
    stack = stack_pop(stack);
    return stack;
}

static void fifo_to_string(struct fifo *queue, char res[])
{
    while (fifo_size(queue) > 0)
    {
        struct elem *head_elem = fifo_head(queue);
        char num[64];

        if (head_elem->token == N)
        {
            strcat(res, my_itoa(head_elem->data, num));
            strcat(res, " ");
        }
        else if (head_elem->token == ADD)
            strcat(res, "+ ");
        else if (head_elem->token == SUB)
            strcat(res, "- ");
        else if (head_elem->token == MULT)
            strcat(res, "* ");
        else if (head_elem->token == DIV)
            strcat(res, "/ ");
        else if (head_elem->token == MODULO)
            strcat(res, "% ");
        else if (head_elem->token == POW)
            strcat(res, "^ ");

        free(head_elem);
        fifo_pop(queue);
    }
}

char *stdn_to_rpn(const char *c, char res[])
{
    if (res)
        res[0] = '\0';

    int error;
    struct elem *elem = parse(c, &error);
    struct stack *stack = NULL;
    struct fifo *queue = fifo_init();

    while (elem)
    {
        if (elem->token == N)
        {
            fifo_push(queue, elem);
        }
        else if (is_operator(elem))
        {
            stack = push_operators_until_lower(stack, elem, queue);
            stack = stack_push(stack, elem);
        }
        else if (elem->token == L_PAR)
        {
            stack = stack_push(stack, elem);
        }
        else if (elem->token == R_PAR)
        {
            stack = process_r_par(stack, queue);
        }
        elem = elem->next;
    }

    while (stack && stack_peek(stack) && is_operator(stack_peek(stack)))
    {
        fifo_push(queue, stack_peek(stack));
        stack = stack_pop(stack);
    }

    fifo_to_string(queue, res);

    free_tokens(elem);
    fifo_destroy(queue);
    free(stack);
    return res;
}

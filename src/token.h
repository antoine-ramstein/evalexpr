#ifndef TOKEN_H
#define TOKEN_H

enum Token
{
    N,
    ADD,
    SUB,
    MULT,
    DIV,
    MODULO,
    POW,
    L_PAR,
    R_PAR,
    NOTHING,

};

struct elem
{
    int data;
    enum Token token;
    struct elem *next;
};

#endif /* ! TOKEN_H */

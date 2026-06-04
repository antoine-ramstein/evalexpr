#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rpn.h"
#include "stdn.h"

static void remove_newline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

static int is_num(char c)
{
    return (c >= 48 && c <= 57);
}

static int is_operator(char c)
{
    return (c == '+' || c == '-' || c == '/' || c == '*' || c == '%' || c == '('
            || c == ')' || c == '^');
}

void add_space(char *str)
{
    char buf[1024];
    size_t len = strlen(str);
    if (len >= sizeof(buf))
        len = sizeof(buf) - 1;
    memcpy(buf, str, len);
    buf[len] = '\0';
    size_t ind = 0;
    for (size_t i = 0; i < len; i++)
    {
        char c = buf[i];
        if (is_num(c))
        {
            str[ind++] = c;
            if (i + 1 < len && !is_num(buf[i + 1]))
                str[ind++] = ' ';
        }
        else if (is_operator(c))
        {
            if (ind > 0 && str[ind - 1] != ' ')
                str[ind++] = ' ';
            str[ind++] = c;
            if (i + 1 < len && buf[i + 1] != ' ')
                str[ind++] = ' ';
        }
        else if (c == ' ')
        {
            if (ind > 0 && str[ind - 1] != ' ')
                str[ind++] = ' ';
        }
        else
        {
            str[ind++] = c;
        }
        if (ind >= sizeof(buf) - 2)
            break;
    }
    if (ind > 0 && str[ind - 1] == ' ')
        ind--;
    str[ind] = '\0';
}

int main(int argc, char *argv[])
{
    char buffer[1024];
    if (argc > 2)
    {
        return 4;
    }
    if (argc == 2 && strcmp(argv[1], "-rpn") != 0)
    {
        return 4;
    }

    char *ptr = fgets(buffer, sizeof(buffer), stdin);
    if (!ptr)
        return 4;
    remove_newline(buffer);
    add_space(buffer);
    if (strlen(buffer) == 0)
        return 0;
    int error = 0;
    int result;

    if (argc == 2 && strcmp(argv[1], "-rpn") == 0)
    {
        result = valuate(buffer, &error);
    }
    else
    {
        char res[64] = { 0 };
        char *test = stdn_to_rpn(buffer, res);
        result = valuate(test, &error);
    }
    if (error)
    {
        fprintf(stderr, "Erreur: %d\n", error);
        return error;
    }
    printf("%d\n", result);
    return 0;
}

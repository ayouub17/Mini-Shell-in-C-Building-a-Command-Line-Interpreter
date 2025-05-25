#include <stddef.h>  // For NULL and size_t
#include <stdio.h>   // For BUFSIZ, stdin, etc.
#include <string.h>  // For strcmp, strtok
#include <unistd.h>  // For isatty, fileno
#include "cell.h"

int status = 0;

t_builtin g_builtin[] = 
{
    {"echo", cell_echo},
    {"env", cell_env},
    {"exit", cell_exit},
    {NULL, NULL}
};

void cell_launch(char **args)
{
    if (Fork() == CELL_JR)
    {
        Execvp(args[0], args);
    }
    else
    {
        Wait(&status);
    }
}

void cell_execute(char **args)
{
    int i;
    const char *curr_builtin;

    if (!args || !args[0])
        return;
    i = 0;

    while ((curr_builtin = g_builtin[i].builtin_name))
    {
        if (!strcmp(args[0], curr_builtin))
        {
            if ((status = g_builtin[i].foo(args)))
                p("%s failed\n", curr_builtin);
            return;
        }
        i++;
    }
    cell_launch(args);
}

char *cell_read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    char cwd[BUFSIZ];

    if (isatty(fileno(stdin)))
    {
        if (status)
            p("🦠" C "[%s]" RED "[%d]" RST "🦠 > ", 
                Getcwd(cwd, BUFSIZ), 
                status);
        else
            p("🦠" C "[%s]" RST "🦠 > ", 
                Getcwd(cwd, BUFSIZ));
    }

    Getline(&line, &bufsize, stdin);
    return line;
}

char **cell_split_line(char *line)
{
    size_t bufsize = BUFSIZ;
    unsigned long position = 0;
    char **tokens = Malloc(bufsize * sizeof(*tokens));

    char *token = strtok(line, SPACE);
    while (token)
    {
        tokens[position++] = token;
        if (position >= bufsize)
        {
            bufsize *= 2;
            tokens = Realloc(tokens, bufsize * sizeof(*tokens));
        }
        token = strtok(NULL, SPACE);
    }
    tokens[position] = NULL;
    return tokens;
}

int main()
{
    SHELL_BANNER;
    char *line;
    char **args;

    while ((line = cell_read_line())) {
        if (!(args = cell_split_line(line))) {
            free(line);
            continue;
        }
        
        if (args[0]) {
            if (!strcmp(args[0], "cd")) {
                if (args[1]) Chdir(args[1]);
                else fprintf(stderr, RED "cd: missing argument\n" RST);
            } else {
                cell_execute(args);
            }
        }
        
        free(line);
        free(args);
    }
    return EXIT_SUCCESS;
}
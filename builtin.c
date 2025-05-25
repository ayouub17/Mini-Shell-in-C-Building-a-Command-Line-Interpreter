#include "cell.h"


int cell_echo(char **args)
{
    p(AYOUB_SIGNATURE " ");  
    
    int start = 1;
    bool newline = true;

    if (!args || !args[0])
        return (1);

    if (args[1] && !strcmp(args[1], "-n")) {
        newline = false;
        start = 2;
    }

    for (int i = start; args[i]; i++) {
        p("%s", args[i]);
        if (args[i + 1])
            p(" ");
    }

    if (newline)
        p("\n");

    return (0);
}

int cell_env(char **args)
{
    extern char **environ;
    
    p(AYOUB_SIGNATURE "\n");  
    
    (void)args;
    if (!environ)
        return (1);
        
    for (int i = 0; environ[i]; i++)
        p("%s\n", environ[i]);
        
    return (0);
}

int cell_exit(char **args)
{
    (void)args;
    dbzSpinnerLoading();  
    exit(EX_OK);
}
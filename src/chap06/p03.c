#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern char **environ;

int setenv(const char *name, const char *value, int overwrite)
{
    if (name == NULL || strchr(name, '=') != NULL || name[0] == '\0')
        return -1;

    if (getenv(name) != NULL && !overwrite)
        return 0;

    size_t name_len = strlen(name);
    size_t value_len = strlen(value);
    size_t length = name_len + 1 + value_len + 1;
    char *string = malloc(length);

    snprintf(string, length, "%s=%s", name, value);

    if (putenv(string) != 0)
        return -1;

    return 0;
}

int unsetenv(const char *name)
{
    if (name == NULL || strchr(name, '=') != NULL || name[0] == '\0')
        return -1;
    
    if (getenv(name) == NULL)
        return 0;

    char **e;
    size_t name_len = strlen(name);
    for (e = environ; *e != NULL; e++)
    {
        if (strncmp(*e, name, name_len) == 0 && (*e)[name_len] == '=')
        {
            for (char **shifting = e; *shifting != NULL; shifting++)
            {
                *shifting = *(shifting + 1);
            }
            e--;
        }
    }

    return 0;
}
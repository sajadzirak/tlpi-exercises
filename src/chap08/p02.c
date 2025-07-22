#include <pwd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

struct passwd *my_getpwnam(const char *name)
{
    struct passwd *pwd;
    
    setpwent();
 
    while ((pwd = getpwent()) != NULL)
        if (strcmp(name, pwd->pw_name) == 0)
        {
            endpwent();
            return pwd;
        }
    
    endpwent();
    return NULL;
}
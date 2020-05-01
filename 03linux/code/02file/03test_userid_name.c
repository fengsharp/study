#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

/*
struct passwd {
               char   *pw_name;       
               char   *pw_passwd;     
               uid_t   pw_uid;        
               gid_t   pw_gid;        
               char   *pw_gecos;      
               char   *pw_dir;        
               char   *pw_shell;      
           };
struct passwd *getpwnam(const char *name);
struct passwd *getpwuid(uid_t uid);
*/

int main()
{
    struct passwd* pwd = getpwnam("mfpsso");
    if (pwd != NULL)
    {
        printf("userid %u\n", pwd->pw_uid);

        if (setuid(pwd->pw_uid) != 0)
        {
            perror("setuid");
            exit(1);
        }
    }

    while (1)
    {
        sleep(100);
    }

    return 0;
}
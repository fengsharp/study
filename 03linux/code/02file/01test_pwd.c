#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <shadow.h>
#include <unistd.h>

int main()
{
    const char* user = "feng";
    const char* pwdSrc = "home";

    struct spwd* shadowLine = getspnam(user);
    char* cryptedPwd = crypt(pwdSrc, shadowLine->sp_pwdp);
    if (strcmp(cryptedPwd, shadowLine->sp_pwdp) == 0) {
        puts("login ok.");
    } else {
        puts("login failed.");
    }

    return 0;
}
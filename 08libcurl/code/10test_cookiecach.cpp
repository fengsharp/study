#include <stdio.h>

#include "CookieCache.h"

int main()
{
    const std::string filePath = "del.txt";
    if (CookieCache::instance()->init(filePath) == false)
    {
        puts("cookie cache init fail.");
    }

    // CookieCache::instance()->addCookie("192.168.154.128", "token1", "value1", 0);
    // CookieCache::instance()->addCookie("192.168.154.129", "token2", "value2", 1588149312);
    // CookieCache::instance()->addCookie("192.168.154.130", "token2", "value2", 1588152994);
    CookieCache::instance()->parseAndAddCookie("192.168.154.131", "Set-Cookie: token3=value3;Expires=1588156213");

    CookieCache::instance()->trySaveData();

    puts("=== all ok. ===");
    return 0;
}
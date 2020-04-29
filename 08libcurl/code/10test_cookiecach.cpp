#include <stdio.h>
#include <assert.h>

#include "CookieCache.h"

void testCookieCach()
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
}

void testUrl2Domain()
{
    assert(CookieCache::instance()->parseDomain("") == "");


    std::string url1 = "http://192.168.154.128:8888/game";
    printf("%s\n", CookieCache::instance()->parseDomain(url1).data());
    assert(CookieCache::instance()->parseDomain(url1) == "192.168.154.128");

    std::string url2 = " http://www.baidu.com";
    printf("%s\n", CookieCache::instance()->parseDomain(url2).data());
    assert(CookieCache::instance()->parseDomain(url2) == "www.baidu.com");
    
    std::string url3 = "http://www.baidu.com/game";
    printf("%s\n", CookieCache::instance()->parseDomain(url3).data());
    assert(CookieCache::instance()->parseDomain(url3) == "www.baidu.com");

    std::string url4 = "http://www.baidu.com:8888/game";
    printf("%s\n", CookieCache::instance()->parseDomain(url4).data());
    assert(CookieCache::instance()->parseDomain(url4) == "www.baidu.com");

    std::string url5 = "www.baidu.com:8888/game";
    printf("%s\n", CookieCache::instance()->parseDomain(url5).data());
    assert(CookieCache::instance()->parseDomain(url5) == "www.baidu.com");
}

int main()
{
    // void testCookieCach();
    testUrl2Domain();

    puts("=== all ok. ===");
    return 0;
}
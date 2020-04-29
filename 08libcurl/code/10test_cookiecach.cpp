#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "CookieCache.h"

void testCookieCach()
{
    const std::string filePath = "del.txt";
    CookieCache::instance()->init(filePath);

    // CookieCache::instance()->addCookie("192.168.154.128", "token1", "value1", 0);
    // CookieCache::instance()->addCookie("192.168.154.129", "token2", "value2", 1588149312);
    // CookieCache::instance()->addCookie("192.168.154.130", "token2", "value2", 1588152994);
    CookieCache::instance()->parseAndAddCookie("192.168.154.131", "Set-Cookie: token3=value3;Expires=1588156213");

    CookieCache::instance()->trySaveData();
}

void testUrl2Domain()
{
    char* ret = CookieCache::parseDomain("");
    assert(ret == nullptr);
    free(ret);

    const char* url1 = "http://192.168.154.128:8888/game";
    ret = CookieCache::parseDomain(url1);
    printf("%s\n", ret);
    assert(strcmp(ret, "192.168.154.128") == 0);
    free(ret);

    const char* url2 = " http://www.baidu.com";
    ret = CookieCache::parseDomain(url2);
    printf("%s\n", ret);
    assert(strcmp(ret, "www.baidu.com") == 0);
    free(ret);
    
    const char* url3 = "http://www.baidu.com/game";
    ret = CookieCache::parseDomain(url3);
    printf("%s\n", ret);
    assert(strcmp(ret, "www.baidu.com") == 0);
    free(ret);

    const char* url4 = "http://www.baidu.com:8888/game";
    ret = CookieCache::parseDomain(url4);
    printf("%s\n", ret);
    assert(strcmp(ret, "www.baidu.com") == 0);
    free(ret);

    const char* url5 = "www.baidu.com:8888/game";
    ret = CookieCache::parseDomain(url5);
    printf("%s\n", ret);
    assert(strcmp(ret, "www.baidu.com") == 0);
    free(ret);
}

int main()
{
    // void testCookieCach();
    testUrl2Domain();

    puts("=== all ok. ===");
    return 0;
}
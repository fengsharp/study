#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include <string>
#include <curl/curl.h>

#include "CookieCache.h"

static std::string domain = "192.168.154.128";

const char *const COOKIE_KEY = "Set-Cookie:";
size_t header_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
    size_t stSize = size * nitems;
    if (stSize > strlen(COOKIE_KEY) && memcmp(buffer, COOKIE_KEY, strlen(COOKIE_KEY)) == 0)
    {
        static const char crlf[2] = {'\r', '\n'};
        if (memcmp(buffer + (stSize - 2), crlf, 2) == 0)
        {
            stSize -= 2;
        }
        std::string str(buffer, stSize);
        //printf("----%s\n", str.c_str());
        CookieCache::instance()->parseAndAddCookie(domain, str);
    }

    return size * nitems;
}


int main()
{
    const std::string filePath = "del.out";
    CookieCache::instance()->init(filePath);

    const std::string url = "http://192.168.154.128:8888/game";
    domain = CookieCache::parseDomain(url.data());
    printf("domain: %s\n", domain.data());

    curl_global_init(CURL_GLOBAL_ALL);

    CURL *curlEasyHandle = curl_easy_init();

    curl_easy_setopt(curlEasyHandle, CURLOPT_URL, url.data());
    curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDS, "name=feng&project=curlstudy");
    curl_easy_setopt(curlEasyHandle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curlEasyHandle, CURLOPT_HEADERFUNCTION, header_callback);

    // std::string strCookie = "Set-Cookie: Token1=AAAA;Domain=192.168.154.128";
    // curl_easy_setopt(curlEasyHandle, CURLOPT_COOKIELIST, strCookie.c_str());
    // strCookie = "Set-Cookie: Token2=BBBB;Domain=192.168.154.128";
    // curl_easy_setopt(curlEasyHandle, CURLOPT_COOKIELIST, strCookie.c_str());
    CookieCache::instance()->setCurlRequestCookie(domain, curlEasyHandle);

    CURLcode rcode = curl_easy_perform(curlEasyHandle);
    if (rcode != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform error:%s\n", curl_easy_strerror(rcode));
    }

    curl_easy_cleanup(curlEasyHandle);

    curl_global_cleanup();
    CookieCache::instance()->trySaveData();

    return 0;
}
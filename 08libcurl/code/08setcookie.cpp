#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include <string>
#include <curl/curl.h>

size_t write_data(void* buf, size_t size, size_t nmemb, void* userp) {
    std::string strBuf(static_cast<char*>(buf), size*nmemb);
    printf("receive size:%lu, nmemb:%lu, data:%s\n", size, nmemb, strBuf.c_str());
    return size * nmemb;
}

int main() {
    curl_global_init(CURL_GLOBAL_ALL);

    CURL* curlEasyHandle = curl_easy_init();

    curl_easy_setopt(curlEasyHandle, CURLOPT_URL, "http://192.168.154.128:8888/game");
    curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDS, "name=feng&project=curlstudy");
    curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curlEasyHandle, CURLOPT_VERBOSE, 1L);
    std::string strCookie = "Set-Cookie: Token1=AAAA;Domain=192.168.154.128";
    curl_easy_setopt(curlEasyHandle, CURLOPT_COOKIELIST, strCookie.c_str());
    strCookie = "Set-Cookie: Token2=BBBB;Domain=192.168.154.128";
    curl_easy_setopt(curlEasyHandle, CURLOPT_COOKIELIST, strCookie.c_str());

    CURLcode rcode = curl_easy_perform(curlEasyHandle);
    if (rcode != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform error:%s\n", curl_easy_strerror(rcode));
    }

    curl_easy_cleanup(curlEasyHandle);

    curl_global_cleanup();

    return 0;
}
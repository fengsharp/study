#include <stdio.h>

#include <curl/curl.h> // /usr/include/curl/curl.h

int main() {
    curl_global_init(CURL_GLOBAL_ALL);

    CURL* curlEasyHandle = curl_easy_init();

    curl_easy_setopt(curlEasyHandle, CURLOPT_URL, "http://192.168.154.128:8888/game");
    curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDS, "name=feng&project=curlstudy");
    CURLcode ret = curl_easy_perform(curlEasyHandle);
    if (ret != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform error: %s\n", curl_easy_strerror(ret));
    }

    curl_easy_cleanup(curlEasyHandle);

    curl_global_cleanup();

    return 0;
}


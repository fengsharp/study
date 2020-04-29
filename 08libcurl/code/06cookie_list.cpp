#include <stdio.h>
#include <string>
#include <vector>

#include <curl/curl.h>

size_t write_data(void *buf, size_t size, size_t nmemb, void *userp)
{
    std::string strBuf(static_cast<char *>(buf), size * nmemb);
    printf("receive size:%lu, nmemb:%lu, data:%s\n", size, nmemb, strBuf.c_str());
    return size * nmemb;
}

//std::string localCookie;
std::vector<std::string> rgLocalCookie;

static void recive_cookie(CURL *curl)
{
    rgLocalCookie.clear();

    struct curl_slist *cookies;
    CURLcode res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "Curl curl_easy_getinfo failed: %s\n", curl_easy_strerror(res));
        return;
    }

    struct curl_slist *nc = cookies;
    int i = 1;
    while (nc)
    {
        rgLocalCookie.push_back(std::string(nc->data));
        //printf("[%d]: %s\n", i, nc->data);
        nc = nc->next;
        i++;
    }

    curl_slist_free_all(cookies);

    for (std::string &item : rgLocalCookie)
    {
        printf("%s\n", item.c_str());
    }
}

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);

    for (int i = 0; i < 2; ++i)
    {
        CURL *curlEasyHandle = curl_easy_init();

        curl_easy_setopt(curlEasyHandle, CURLOPT_URL, "http://127.0.0.1:8888/game");
        curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDS, "name=feng&project=curlstudy");
        curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curlEasyHandle, CURLOPT_COOKIELIST, "");

        for (std::string &item : rgLocalCookie)
        {
            CURLcode res = curl_easy_setopt(curlEasyHandle, CURLOPT_COOKIELIST, item.c_str());
            if (res != CURLE_OK)
            {
                fprintf(stderr, "Curl curl_easy_setopt failed: %s\n", curl_easy_strerror(res));
                return -1;
            }
        }

        CURLcode rcode = curl_easy_perform(curlEasyHandle);
        if (rcode != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform error:%s\n", curl_easy_strerror(rcode));
        }
        recive_cookie(curlEasyHandle);

        curl_easy_cleanup(curlEasyHandle);
    }

    curl_global_cleanup();

    return 0;
}
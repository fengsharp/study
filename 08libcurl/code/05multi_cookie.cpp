#include <stdio.h>
#include <string>

#include <curl/curl.h>

size_t write_data(void* buf, size_t size, size_t nmemb, void* userp) {
	int* pIndex = static_cast<int*>(userp);
	std::string strBuf(static_cast<char*>(buf), size * nmemb);
	printf("index:%d receive size:%lu, nmemb:%lu, data:%s\n", *pIndex, size, nmemb, strBuf.c_str());
	return size * nmemb;
}

void addTransfer(CURLM* curlMultiHandle, int index) {
	CURL* curlEasyHandle = curl_easy_init();
	curl_easy_setopt(curlEasyHandle, CURLOPT_URL, "http://127.0.0.1:8888/game");
	curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDS, "name=feng&project=curlstudy");
	curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curlEasyHandle, CURLOPT_COOKIEFILE, "./mycookie.txt");
    curl_easy_setopt(curlEasyHandle, CURLOPT_COOKIEJAR, "./mycookie.txt");
	
	int* pIndex = new int;
	*pIndex = index;
	curl_easy_setopt(curlEasyHandle, CURLOPT_PRIVATE, pIndex);
	curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEDATA, pIndex);
	curl_multi_add_handle(curlMultiHandle, curlEasyHandle);
}

int main()
{
	const long maxConnects = 3;
	const long totalRequsts = 10;

	curl_global_init(CURL_GLOBAL_ALL);

	CURL* curlMultiHandle = curl_multi_init();
	curl_multi_setopt(curlMultiHandle, CURLMOPT_MAXCONNECTS, maxConnects);

	int connCount = 0;
	while (connCount < maxConnects) {
		addTransfer(curlMultiHandle, connCount);
		++connCount;
	}

	int stillAlive = 1;
	bool needWait = (connCount < totalRequsts);
	while (stillAlive || needWait)
	{
		needWait = false;

		curl_multi_perform(curlMultiHandle, &stillAlive);

		CURLMsg* msg;
		int msgs_left = -1;
		while ((msg = curl_multi_info_read(curlMultiHandle, &msgs_left))) {
			if (msg->msg == CURLMSG_DONE) {
				CURL* easyHandle = msg->easy_handle;
				int* pIndex;
				curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &pIndex);
				fprintf(stderr, "R: %d - %s <%d>\n", msg->data.result, curl_easy_strerror(msg->data.result), *pIndex);
				curl_multi_remove_handle(curlMultiHandle, easyHandle);
				curl_easy_cleanup(easyHandle);

				*pIndex = -1;
				delete pIndex;
			}
			else {
				fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
			}

			if (connCount < totalRequsts) {
				addTransfer(curlMultiHandle, connCount);
				++connCount;
				needWait = true;
			}
		}

		if (stillAlive) {
			printf("wait for a monment...\n");
			curl_multi_wait(curlMultiHandle, NULL, 0, 1000, NULL);
		}
	} 

	curl_multi_cleanup(curlMultiHandle);

	curl_global_cleanup();

	return 0;
}
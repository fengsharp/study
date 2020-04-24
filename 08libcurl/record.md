### 1. 资源
- https://curl.haxx.se/libcurl/
- https://blog.csdn.net/jgood/article/details/4787670

### 2. libcurl overview
- libcurl is a url transfer library. surport ftp file http

### 3. step
https://curl.haxx.se/libcurl/c/  
- the libcurl tutorial. https://curl.haxx.se/libcurl/c/libcurl-tutorial.html
- c example source codes. https://curl.haxx.se/libcurl/c/example.html

### 4. start libcurl tutorial
1. 首先用 curl_global_init 初始化curl全局功能. CURL_GLOBAL_ALL初始化所有内部子模块。
2. 在app生命周期结束时，调用 curl_global_cleanup. 整个app生命周期期间，curl_global_init | curl_global_cleanup 都只应该被调用一次
3. 通过 curl_version_info 调用可以获取当前curl版本支持哪些操作
4. libcurl提供两种方式，一种是阻塞的同步方式的 curl_easy, 一种是单线程多通道传输接口 multi interface
5. Easy handle 一个线程一个easy handle. Handle可以视为一个对象实例, Easy handle看作curl的一个easy类型的实例
    - CURL* curlEasyHandle = curl_easy_init(); // return curl easy handle
    - curl发送数据-接收数据，完事后，记得清空 curl_easy_cleanup(curlEasyHandle)
    - curl_easy_setopt 相当于给curlEasyHandle对象设置属性
    - 可以使用curl_easy_reset 清空原有属性设置
    - 属性 CURLOPT_URL curl_easy_setopt(curlEasyHandle, CURLOPT_URL, "http://domain:port/uri")
    - 可以绑定请求返回的结果回调方法，方法
    ```c
    size_t wirte_data(void* buffer, size_t size, size_t nmemb, void* userp);
    // 要想回调到write_data，需要预先设置回调
    curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEFUNCTION, write_data);
    // 可以设置write_data里面的userp
    curl_easy_setopt(curlEasyHandle, CURLOPT_WRITEDATA, &internal_struct);
    // 上面的internal_struct 仅仅起到指针的传递作用
    /*
    应用场景demo：将打开的一个文件传递到回调里面，在回调里面追加到文件里面，然后关闭
    如果没有设置write function，curl内部有一个默认的fuction，将结果输出到stdout
    */
    ```
    - 设置好curlEasyHandle属性后，curl_easy_perform(curlEasyHandle) 按照设置的属性执行传输业务
6. upload data to remote
    - CURL* curlEasyHandle = curl_easy_init();
    - curl_easy_setopt(curlEasyHandle, CURLOPT_READFUNCTION, read_function)
    ```c
    size_t read_function(char* bufptr, size_t size, size_t niterms, void* userp);
    // 可以设置需要传递的 userp
    curl_easy_setopt(curlEasyHandle, CURLOPT_READDATA, &filedata);
    // 同时告诉curl，要上传数据
    curl_easy_setopt(curlEasyHandle, CURLOPT_UPLOAD, 1L);
    /*
    curlEasyHandle会从read_function回调中获取数据，当返回的值为0，说明需要上传数据结束
    */
    ```
7. HTTP-POST-form
    ```c
    CURL* curlEasyHandle = curl_easy_init();
    char* data = "name=feng&project=curlstudy";
    curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curlEasyHandle, CURLOPT_URL, "http://192.168.154.128:888/game);
    curl_easy_perform(curlEasyHandle);
    curl_easy_cleanup(curlEasyHandle);
    ```

8. HTTP-POST-data
    ```c
    CURL* curlEasyHandle = curl_easy_init();
    char* data = "name=feng&project=curlstudy";
    curl_easy_setopt(curlEasyHandle, CURLOPT_URL, "...");
    curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curlEasyHandle, CURLOPT_POSTFIELDSSIZE, strlen(data));
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: text/xml");
    curl_easy_setopt(curEAsyHandle, CURLOPT_HTTPHEADER, headers);
    curl_easy_perform(curlEasyHandle);
    curl_slist_free_all(curlEasyHandle);
    ```

9. easy handle perform后，会保持连接一段时间，下次可以复用连接。可以设置禁用
    - set CURLOPT_FRESH_CONNECT 1
    - SET CURLOPT_FORBID_REUSE 1

10. multi interface
    - easy 是一次传输一个文件，完成后返回的同步接口
    - multi 相当于easy对象集合


### 5. vs设置
    - 常规 附件包含目录 /usr/include/curl
    - link 附加库目录 /usr/lib64
    - link input 库依赖项 curl

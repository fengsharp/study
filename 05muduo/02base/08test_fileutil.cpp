#include <stdio.h>
#include <string.h>

#include <string>

#include "FileUtil.h"
#include "Logging.h"

int main()
{
    std::string content;
    int64_t size;
    int err = FileUtil::readFile("/proc/self/cmdline", 1024, &content, &size);

    char buf[1024] = {'\0'};
    snprintf(buf, sizeof(buf) - 1, "err:%d size:%ld content:", err, size);
    FileUtil::AppendFile appendFile("del.out");
    appendFile.append(buf, strlen(buf));
    if (err != 0)
    {
        const char * errMsg = strerror_tl(err);
        appendFile.append(errMsg, strlen(errMsg));
    }
    else
    {
        appendFile.append(content.data(), content.size());
    }
    appendFile.append("\n", 1);
    appendFile.flush();


    return 0;
}
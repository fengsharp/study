#include <iostream>
#include <stdio.h>
#include <string.h>

#include "GzipFile.h"

int main()
{
    const char * filename = "/tmp/gzipfile_test.gz";
    ::unlink(filename);
    const char data[] = "123456789012345678901234567890123456789012345678901234567890\n";
    {
        GzipFile writer = GzipFile::openForAppend(filename);
        if (writer.valid())
        {
            std::cout << "tell " << writer.tell() << std::endl;
            std::cout << "wrote " << writer.write(data) << std::endl;
            std::cout << "tell " << writer.tell() << std::endl;
        }
    }

    {
        printf("testing reader\n");
        GzipFile reader = GzipFile::openForRead(filename);
        if (reader.valid())
        {
            char buf[256];
            std::cout << "tell " << reader.tell() << std::endl;
            int nr = reader.read(buf, sizeof buf);
            printf("read %d\n", nr);
            if (nr >= 0)
            {
                buf[nr] = '\0';
                printf("data %s", buf);
            }
            std::cout << "tell " << reader.tell() << std::endl;
            if (strncmp(buf, data, strlen(data)) != 0)
            {
                printf("failed!!!\n");
                abort();
            }
            else
            {
                printf("PASSED\n");
            }
        }
    }

    {
        GzipFile writer = GzipFile::openForWriteExclusive(filename);
        if (writer.valid() || errno != EEXIST)
        {
            printf("FAILED\n");
        }
    }

    return 0;
}

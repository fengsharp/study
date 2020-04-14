#include <limits>
#include <stdint.h>
#include <assert.h>
#include <string>

using namespace std;

#include "LogStream.h"

void testLogStreamBooleans() {
    LogStream os;
    LogStream::Buffer& buf = os.buffer();
    assert(buf.toString() == std::string(""));
    os << true;
    assert(buf.toString() == std::string("1"));
    os << '\n';
    assert(buf.toString() == std::string("1\n"));
    os << false;
    assert(buf.toString() == std::string("1\n0"));
}

void testLogStreamIntegers() {
    LogStream os;
    LogStream::Buffer& buf = os.buffer();
    assert(buf.toString() == std::string(""));

    os << 1;
    assert(buf.toString() == std::string("1"));
    os << 0;
    assert(buf.toString() == std::string("10"));
    os << -1;
    assert(buf.toString() == std::string("10-1"));

    os.resetBuffer();
    os << 0 << " " << 123 << 'x' << 0x64;
    assert(buf.toString() == std::string("0 123x100"));
}

void testLogStreamFloats() {
    LogStream os;
    LogStream::Buffer& buf = os.buffer();

    os << 0.0;
    assert(buf.toString() == std::string("0"));
    os.resetBuffer();

    os << 1.0;
    assert(buf.toString() == std::string("1"));
    os.resetBuffer();

    os << 0.1;
    assert(buf.toString() == std::string("0.1"));
    os.resetBuffer();

    os << 0.05;
    assert(buf.toString() == std::string("0.05"));
    os.resetBuffer();

    os << 0.15;
    assert(buf.toString() == std::string("0.15"));
    os.resetBuffer();
}

void testLogStreamVoid() {
    LogStream os;
    LogStream::Buffer& buf = os.buffer();

    os << static_cast<void*>(0);
    assert(buf.toString() == std::string("0x0"));
    os.resetBuffer();

    os << reinterpret_cast<void*>(8888);
    assert(buf.toString() == std::string("0x22B8"));
    os.resetBuffer();
}

void testLogStreamStrings() {
    LogStream os;
    LogStream::Buffer& buf = os.buffer();

    os << "Hello ";
    assert(buf.toString() == std::string("Hello "));

    string chenshuo = "Shuo Chen";
    os << chenshuo;
    assert(buf.toString() == std::string("Hello Shuo Chen"));
}

void testLogStreamFmts() {
    LogStream os;
    LogStream::Buffer& buf = os.buffer();

    os << Fmt("%4d", 1);
    assert(buf.toString() == std::string("   1"));
    os.resetBuffer();

    os << Fmt("%4.2f", 1.2);
    assert(buf.toString() == std::string("1.20"));
    os.resetBuffer();

    os << Fmt("%4.2f", 1.2) <<  Fmt("%4d", 43);
    assert(buf.toString() == std::string("1.20  43"));
    os.resetBuffer();
}

void testLogStreamLong() {
    LogStream os;
    LogStream::Buffer& buf = os.buffer();

    for (int i = 0; i < 399; ++i)
    {
        os << "123456789 ";
        assert(buf.length() == (10*(i+1)));
        assert(buf.avail() == (4000 - 10*(i+1)));
    }

    os << "abcdefghi ";
    assert(buf.length() == 3990);
    assert(buf.avail() == 10);

    os << "abcdefghi";
    assert(buf.length() == 3999);
    assert(buf.avail() == 1);
}

int main() {
    testLogStreamBooleans();
    testLogStreamIntegers();
    testLogStreamFloats();
    testLogStreamVoid();
    testLogStreamStrings();
    testLogStreamFmts();
    testLogStreamLong();

    puts("=== all over. ===");
    return 0;
}
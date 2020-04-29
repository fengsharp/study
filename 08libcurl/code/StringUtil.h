#ifndef Pirate_stringUtil_h
#define Pirate_stringUtil_h

#include <vector>
#include <string>
#include <sstream>
#include <string.h>

using namespace std;

string num2str(unsigned int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}

string num2str(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}

string num2str(long i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}

string num2str(long long i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}

int str2num(string s)
{
    int num = 0;
    stringstream ss(s);
    ss >> num;
    return num;
}

long str2long(string s)
{
    long num = 0;
    stringstream ss(s);
    ss >> num;
    return num;
}

long long str2longlong(string s)
{
    long long num = 0;
    stringstream ss(s);
    ss >> num;
    return num;
}

void stringSplit(string src, const char* token, vector<string>& vect)
{
    if (src.empty()) {
        return;
    }
    int nend=0;
    int nbegin=0;
    while (nend != -1)
    {
        nend = src.find(token, nbegin);
        if(nend == -1)
        {
            vect.push_back(src.substr(nbegin, src.length()-nbegin));
        }
        else
        {
            vect.push_back(src.substr(nbegin, nend-nbegin));
        }
        nbegin = nend + strlen(token);
    }
}

// trim from start
string &ltrim(string &s) 
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
    return s;
}

// trim from end
string &rtrim(string &s) 
{
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

// trim from both ends
string &trim(string &s) 
{
    return ltrim(rtrim(s));
}

#endif
#ifndef COOKIE_CACHE_H_
#define COOKIE_CACHE_H_

#include <map>
#include <string>
#include <curl/curl.h>

// domain, name, value, expires
// 域名 过期时间(UTC timestamp, seconds) 名 值
// 服务端如果设置域名 Domain=<domain-value>  如果不设置，默认为本url的domain
// 服务端如果设置过期时间 Expires=<utc timestamp> 秒单位 如果不设置，默认不存盘，只在内存存在且不过期处理
// Set-Cookie Domain Expires 区分大小写（前端字符串对比简化）
// 服务端设置
/*
'Set-Cookie': ['testcookie1=100;Domain=192.168.154.128;Expires=1588145538', 
               'testcookie2=200;Domain=192.168.154.129;Expires=1588145538', ]
*/

/*
保证每次发请求的时候检查一下过期的别发就行了
然后存盘的时候检查一下过期的不存，启动读取的时候也扔掉过期的，这种。
不用特意保证它过期的时候立刻就删，然后收到回应后也不用检查现有的是不是有过期的，
刚收到的那些检查一下就行了，如果真有过期的就去库里删掉，这也是服务器删 cookie 的一种方法。
*/

class CookieCache
{
public:
    static CookieCache* instance();
private:
    CookieCache();
    CookieCache(const CookieCache &);
	CookieCache & operator = (const CookieCache &);
public:
    bool init(const std::string& localFilePath);
    void trySaveData();
    // format: Set-Cookie: k=v;Domain=...;Expires=...
    // can not set anything but this(k=v; Domain; Expires), and (k=v) in the first position
    // can ignore Domain Expires, if set Domain or Expires, value not set null
    // can not include \r\n
    void parseAndAddCookie(const std::string reqDomain, const std::string& strCookie);
private:
    bool loadData();
    bool saveData();

    // expires:utc timestamp(seconds). set 0, not save local
    // expires = 0, mark not save, only exists during this login(memory)
    void addCookie(const std::string& domain, const std::string& cookieKey, const std::string& cookieValue, long long expires=0);
private:
    static CookieCache* s_pInstance;

    std::string m_strFile;

    struct CookieProperty
    {
        CookieProperty() = default;
        CookieProperty(const std::string& cookieValue, long long cookieExpires)
        : value(cookieValue), expires(cookieExpires)
        {
        }
        std::string value;
        long long expires;
    };
    // [domain, [cookieKey, CookieProperty]]
    std::map<std::string, std::map<std::string, CookieProperty>> m_mapLocalCookie;
    bool m_bSave = false;
};

#endif
#include "CookieCache.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <regex>

#include "StringUtil.h"

CookieCache* CookieCache::s_pInstance = nullptr;
CookieCache* CookieCache::instance()
{
    if (s_pInstance == nullptr)
    {
        s_pInstance = new CookieCache();
    }
    return s_pInstance;
}

CookieCache::CookieCache()
{
}

void CookieCache::init(const std::string &localFilePath)
{
    m_strFile = localFilePath;
    loadData();
}


void CookieCache::loadData()
{
    FILE* file = fopen(m_strFile.c_str(), "r");
    if (file == NULL) // if file not exist, return
    {
        return;
    }

    // domain \t key \t value \t expires \n
    long long now = time(NULL);
    char buf[BUFSIZ];
    while(fgets(buf, BUFSIZ, file) != NULL)
    {
        size_t len = strlen(buf);
        if (buf[len-1] == '\n')
        {
            len -= 1;
        }
        std::string strLine(buf, len);
    
        std::vector<std::string> splits;
        stringSplit(strLine, "\t", splits);
        if (splits.size() != 4)
        {
            continue;
        }
        long long expires = str2longlong(splits[3]);
        if (now < expires)
        {
            addCookie(splits[0], splits[1], splits[2], expires);
        }
    }

    fclose(file);

    // try clear expires
    trySaveData();
}

bool CookieCache::saveData()
{
    FILE* file = fopen(m_strFile.c_str(), "w");
    if (file == NULL)
    {
        return false;
    }

    long long now = time(NULL);
    for (auto& domainItem : m_mapLocalCookie)
    {
        const std::string& strDomain = domainItem.first;
        for(auto& cookieItem : domainItem.second)
        {
            long long expires = cookieItem.second.expires;
            if (expires <= now)
            {
                continue;
            }
            // domain key value expires
            fprintf(file, "%s\t%s\t%s\t%lld\n", strDomain.c_str(), 
                                                cookieItem.first.c_str(), 
                                                cookieItem.second.value.c_str(),
                                                expires);
        }
    }

    fclose(file);
    return true;
}

void CookieCache::trySaveData()
{
    if (m_bSave)
    {
        saveData();
        m_bSave = false;
    }
}

void CookieCache::addCookie(const std::string& domain, const std::string& cookieKey, const std::string& cookieValue, long long expires/*=0*/)
{
    if (domain.empty() || cookieKey.empty())
    {
        return;
    }

    long long now = time(NULL);
    if (expires > 0 && expires <= now)
    {
        return;
    }

    bool dataChanged = false;
    std::map<std::string, std::map<std::string, CookieProperty>>::iterator findDomain = m_mapLocalCookie.find(domain);
    if (cookieValue.empty()) // delete
    {
        if (findDomain != m_mapLocalCookie.end())
        {
            std::map<std::string, CookieProperty>::iterator cookieItem = findDomain->second.find(cookieKey);
            if (cookieItem != findDomain->second.end())
            {
                findDomain->second.erase(cookieItem);
                dataChanged = true; // data delete
            }
        }
    }
    else // set
    {
        if (findDomain == m_mapLocalCookie.end())
        {
            m_mapLocalCookie[domain].insert(std::pair<std::string, CookieProperty>(cookieKey, CookieProperty(cookieValue, expires)));
            dataChanged = true; // data add
        }
        else
        {
            std::map<std::string, CookieProperty>::iterator cookieItem = findDomain->second.find(cookieKey);
            if (cookieItem == findDomain->second.end())
            {
                findDomain->second[cookieKey] = CookieProperty(cookieValue, expires);
                dataChanged = true;
            }
            else
            {
                if (cookieItem->second.value != cookieValue)
                {
                    findDomain->second[cookieKey] = CookieProperty(cookieValue, expires);
                    dataChanged = true;
                } // else value not change, expires change, ignore
            }
            
        }
    }

    if (!m_bSave && (expires > 0 && dataChanged))
    {
        m_bSave = true;
    }
}

void CookieCache::parseAndAddCookie(const std::string& reqDomain, const std::string& strCookie)
{
    // Set-Cookie: k=v;Domain=...;Expires=...
    static const std::string COOKIE_SET_MARK = "Set-Cookie:";
    if (strCookie.find(COOKIE_SET_MARK) != 0)
    {
        return;
    }

    // cookieFormat = [ k=v;Domain=...;Expires=...]
    std::string cookieFormat = strCookie.substr(COOKIE_SET_MARK.length());
    std::vector<std::string> splits;
    stringSplit(cookieFormat, ";", splits);
    if (splits.empty())
    {
        return;
    }

    // name-value
    std::vector<std::string> nameValues;
    stringSplit(splits[0], "=", nameValues);
    if (nameValues.size() != 2)
    {
        return;
    }

    std::string cookieKey = trim(nameValues[0]);
    std::string cookieValue = trim(nameValues[1]);

    std::string domain = reqDomain;
    long long expires = 0;
    // Domain Expires
    for (size_t i=1; i<splits.size(); ++i)
    {
        std::vector<std::string> tmpKeyValue;
        stringSplit(splits[i], "=", tmpKeyValue);
        if (tmpKeyValue.size() != 2)
        {
            continue;
        }

        std::string key = trim(tmpKeyValue[0]);
        std::string value = trim(tmpKeyValue[1]);
        if (key == "Domain")
        {
            domain = value;
        }
        else if (key == "Expires")
        {
            expires = str2longlong(value);
        }
    }
    addCookie(domain, cookieKey, cookieValue, expires);
}

void CookieCache::setCurlRequestCookie(const std::string& reqDomain, CURL* curlHandle)
{
    std::map<std::string, std::map<std::string, CookieProperty>>::iterator findItem = m_mapLocalCookie.find(reqDomain);
    if (findItem == m_mapLocalCookie.end())
    {
        return;
    }

    long long now = time(NULL);
    std::map<std::string, CookieProperty>& mapCookies = findItem->second;
    for (auto& cookieItem : mapCookies)
    {
        if (cookieItem.second.value.empty() || cookieItem.second.expires <= now)
        {
            continue;
        }
        else
        {
            std::string strCookie("Set-Cookie: ");
            strCookie.append(cookieItem.first);
            strCookie.append("=");
            strCookie.append(cookieItem.second.value);
            printf("### set cookie string:%s\n", strCookie.data());
            curl_easy_setopt(curlHandle, CURLOPT_COOKIELIST, strCookie.c_str());
        }
    }
}

// http://192.168.154.128:8888/game
// http://www.baidu.com
// http://www.baidu.com/game
// http://www.baidu.com:8888/game
std::string CookieCache::parseDomain(const std::string& url)
{
    std::string ret;
    if (url.empty())
    {
        return ret;
    }

    size_t startIndex = 0;
    static const std::string URL_PRIFIX = "http://";
    size_t prefixPos = url.find(URL_PRIFIX);
    if (prefixPos != std::string::npos)
    {
        startIndex = prefixPos + URL_PRIFIX.length();
    }
    
    const char* cursor = url.data() + startIndex;
    while (*cursor != '\0' && *cursor != '/' && *cursor != ':')
    {
        ++cursor;
    }
    size_t stopIndex = cursor - url.data();

    if (stopIndex > startIndex)
    {
        ret = url.substr(startIndex, (stopIndex-startIndex));
    }
    return ret;
}
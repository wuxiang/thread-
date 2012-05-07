#ifndef __STDX_NETDB_H
#define __STDX_NETDB_H


// Posix header files
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // inet_pton, inet_addr
#include <unistd.h>
#include <netdb.h>
#include <ifaddrs.h>

// C++ 98
#include <vector>

// stdx header files
#include "stdx_memory.h"
#include "stdx_string.h"


namespace stdx {

struct netdb
{
    // WARN: src must be numbers-and-dots notation, if input is invalid, return value
    //       is INADDR_NONE(-1), otherwise, return value is network order address.
    static inline in_addr_t
    inet_addr(const std::string& src)
    {
        return ::inet_addr(src.c_str());
    }

    static inline int
    inet_pton(const std::string& src, in_addr& dst)
    {
        return ::inet_pton(AF_INET, src.c_str(), &dst);
    }

    static inline int
    inet_pton(const std::string& src, in6_addr& dst)
    {
        return ::inet_pton(AF_INET6, src.c_str(), &dst);
    }

    static inline std::string
    inet_ntop(const struct in_addr& addr)
    {
        char buf[INET_ADDRSTRLEN] = { '\0' };
        std::string dst;
        if (::inet_ntop(AF_INET, &addr, buf, sizeof(buf)) != NULL)
            dst = buf;
        return dst;
    }

    static inline std::string
    inet_ntop(const struct in6_addr& addr)
    {
        char buf[INET6_ADDRSTRLEN] = { '\0' };
        std::string dst;
        if (::inet_ntop(AF_INET6, &addr, buf, sizeof(buf)) != NULL)
            dst = buf;
        return dst;
    }

    static inline int
    getnameinfo(sockaddr* sa, std::string& hostname, std::string& servname)
    {
        char hbuf[NI_MAXHOST] = { 0 };
        char sbuf[NI_MAXSERV] = { 0 };

        socklen_t len = (sa->sa_family == AF_INET) ?
            sizeof(sockaddr_in) : sizeof(sockaddr_in6);
        int ret = ::getnameinfo(sa, len, hbuf, sizeof(hbuf),
                sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
        if (ret == 0)
        {
            hostname = hbuf;
            servname = sbuf;
        }

        // another method to get hostname and servname
    //  if (sa->sa_family == AF_INET)
    //  {
    //      struct sockaddr_in* sin = reinterpret_cast<struct sockaddr_in*>(sa);
    //      hostname = netdb::inet_ntop(sin->sin_addr);
    //      servname = to_string(ntohs(sin->sin_port));
    //  }
    //  else if (sa->sa_family == AF_INET6)
    //  {
    //      struct sockaddr_in6* sin6 = reinterpret_cast<struct sockaddr_in6 *>(sa);
    //      hostname = netdb::inet_ntop(sin6->sin6_addr);
    //      servname = to_string(ntohs(sin6->sin6_port));
    //  }

        return ret;
    }
};


template <>
struct free_functor<struct addrinfo>
{
    void
    operator()(struct addrinfo* p) const
    {
        if (p != NULL)
        {
            ::freeaddrinfo(p);
        }
    }
};

template <>
struct free_functor<struct ifaddrs>
{
    void
    operator()(struct ifaddrs* p) const
    {
        if (p != NULL)
        {
            ::freeifaddrs(p);
        }
    }
};


inline bool
stdx_getifaddrs_v4(std::vector<in_addr_t>& hosts)
{
    struct ifaddrs *ifaddr;
    if (getifaddrs(&ifaddr) == -1)
        return false;
    auto_delete<struct ifaddrs, free_functor> aai(ifaddr);

    for (struct ifaddrs* ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        int family = ifa->ifa_addr->sa_family;
        if (family == AF_INET && !(ifa->ifa_flags & IFF_LOOPBACK) && (ifa->ifa_flags & IFF_UP))
        {
            struct sockaddr_in *sin = reinterpret_cast<struct sockaddr_in *>(ifa->ifa_addr);
            hosts.push_back(sin->sin_addr.s_addr);
        }
    }

    return true;
}

inline bool
stdx_getifaddrs_v4(std::vector<std::string>& hosts)
{
    struct ifaddrs *ifaddr;
    if (getifaddrs(&ifaddr) == -1)
        return false;
    auto_delete<struct ifaddrs, free_functor> aai(ifaddr);

    for (struct ifaddrs* ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        int family = ifa->ifa_addr->sa_family;
        if (family == AF_INET && !(ifa->ifa_flags & IFF_LOOPBACK) && (ifa->ifa_flags & IFF_UP))
        {
            std::string strhost, strserv;
            if (netdb::getnameinfo(ifa->ifa_addr, strhost, strserv) != 0)
                return false;
            hosts.push_back(strhost);
        }
    }

    return true;
}

// only get the first addr
inline std::string
stdx_getifaddr_v4()
{
    std::vector<std::string> hosts;
    bool result = stdx_getifaddrs_v4(hosts);
    if(result && !hosts.empty())
    {        
        return hosts.front();
    }
    else
    {
        return std::string();
    }    
    //  return "127.0.0.1";
}


} // namespace stdx


#endif // __STDX_NETDB_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

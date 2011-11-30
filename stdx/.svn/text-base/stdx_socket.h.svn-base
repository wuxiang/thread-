#ifndef __STDX_SOCKET_H
#define __STDX_SOCKET_H


// Posix header files
#include <sys/types.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/socket.h> // socket
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <ifaddrs.h>

// C 89 header files
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

// C++ 98 header files
#include <stdexcept>
#include <string>

// stdx header files
#include "stdx_buffer.h"
#include "stdx_netdb.h"


namespace stdx {


class socket_stream
{
private:
    int m_fd;

public:
    explicit socket_stream(int sockfd = -1) : m_fd(sockfd)
    { }

    ~socket_stream()
    {
        m_fd = -1;
    }

    int close()
    {
        int ret = 0;
        if (m_fd > 0)
        {
            ret = ::close(m_fd);
            m_fd = -1;
        }
        return ret;
    }

    bool is_valid() const
    {
        return m_fd >= 0;
    }

    int sockfd() const
    {
        return m_fd;
    }

    ssize_t readn(void *vptr, size_t n) const
    {
        assert(is_valid());
        char *ptr = static_cast<char*>(vptr);
        ssize_t nread;
        size_t nleft = n;
        while (nleft > 0)
        {
            if ((nread = read(m_fd, ptr, nleft)) < 0)
            {
                if (errno == EINTR)
                    nread = 0;      /* and call read() again */
                else
                    return -1;
            }
            else if (nread == 0)
                break;              /* EOF */
            nleft -= nread;
            ptr   += nread;
        }
        return (n - nleft);      /* return >= 0 */
    }

    ssize_t writen(const void *vptr, size_t n) const
    {
        assert(is_valid());
        const char* ptr = static_cast<const char *>(vptr);
        ssize_t nwritten;
        size_t nleft = n;
        while (nleft > 0)
        {
            if ((nwritten = write(m_fd, ptr, nleft)) <= 0)
            {
                if (nwritten < 0 && errno == EINTR)
                    nwritten = 0;       /* and call write() again */
                else
                    return -1;         /* error */
            }
            nleft -= nwritten;
            ptr   += nwritten;
        }
        return(n);
    }

    // TODO: wrong method
    ssize_t read_uint64(uint64_t& val) const
    {
        int ret = readn(&val, sizeof(val));
        if (ret == sizeof(val))
        {
            uint32_t* lval = reinterpret_cast<uint32_t*>(&val);
            uint32_t* hval = lval + 1;
            *lval = ntohl(*lval);
            *hval = ntohl(*hval);
        }
        return ret;
    }

    ssize_t write_uint64(uint64_t val) const
    {
        uint32_t* lval = reinterpret_cast<uint32_t*>(&val);
        uint32_t* hval = lval + 1;
        *lval = htonl(*lval);
        *hval = htonl(*hval);
        int ret = writen(&val, sizeof(val));
        return ret;
    }

    ssize_t read_uint32(uint32_t& val) const
    {
        int ret = readn(&val, sizeof(val));
        if (ret == sizeof(val))
            val = ntohl(val);
        return ret;
    }

    ssize_t write_uint32(uint32_t val) const
    {
        val = htonl(val);
        int ret = writen(&val, sizeof(val));
        return ret;
    }

    ssize_t read_uint16(uint16_t& val) const
    {
        int ret = readn(&val, sizeof(val));
        if (ret == sizeof(val))
            val = ntohs(val);
        return ret;
    }

    ssize_t write_uint16(uint16_t val) const
    {
        val = htons(val);
        int ret = writen(&val, sizeof(val));
        return ret;
    }

    ssize_t read_ipaddr(in_addr_t& ipaddr) const
    {
        return read_uint32(ipaddr);
    }

    ssize_t write_ipaddr(in_addr_t ipaddr) const
    {
        return write_uint32(ipaddr);
    }

    ssize_t read_port(in_port_t& port) const
    {
        return read_uint16(port);
    }

    ssize_t write_port(in_port_t port) const
    {
        return write_uint16(port);
    }

    bool read_buffer(void*& buf, uint32_t& len) const
    {
        if (!read_uint32(len))
            return false;

        if (len == 0)
            return true;

        buf = malloc(len);
        if (buf == 0)
            return false;

        int ret = readn(buf, len);
        if (ret != static_cast<int>(len))
            return false;

        return true;
    }

    bool write_buffer(const void* buf, uint32_t len) const
    {
        if (!write_uint32(len))
            return false;

        if (len == 0)
            return true;

        assert(buf != NULL);
        if (buf == NULL)
            return false;

        int ret = writen(buf, len);
        if (ret != static_cast<int>(len))
            return false;

        return true;
    }

    bool read_buffer(auto_buffer& buf) const
    {
        uint32_t len = 0;
        void* ptr = NULL;
        bool ret = this->read_buffer(ptr, len);
        if (ret)
            buf.reset(ptr, len);
        return ret;
    }

    bool write_buffer(const auto_buffer& buf) const
    {
        assert((buf.size() == 0 && buf.data() == NULL) ||
               (buf.size() != 0 && buf.data() != NULL));

        return this->write_buffer(buf.data(), buf.size());
    }

    bool read_string(std::string& str) const
    {
        auto_buffer buf;
        if (!this->read_buffer(buf))
            return false;

        str.assign(static_cast<char*>(buf.data()), buf.size());
        return true;
    }

    bool write_string(const std::string& str) const
    {
        return this->write_buffer(str.c_str(), str.size());
    }

    bool getsockname(std::string& hostname, std::string& servname) const
    {
        assert(is_valid());

        struct sockaddr_storage ss;
        socklen_t addrlen = sizeof(ss);

        if (::getsockname(m_fd, (struct sockaddr*)&ss, &addrlen) < 0)
        {
            perror("getsockname");
            return false;
        }

        struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(&ss);

        return getaddrport(sa, hostname, servname);
    }

    bool getpeername(std::string& hostname, std::string& servname) const
    {
        assert(is_valid());

        struct sockaddr_storage ss;
        socklen_t addrlen = sizeof(ss);

        if (::getpeername(m_fd, (struct sockaddr*)&ss, &addrlen) < 0)
        {
            perror("getpeername");
            return false;
        }

        struct sockaddr* sa = reinterpret_cast<struct sockaddr*>(&ss);

        return getaddrport(sa, hostname, servname);
    }

private:
    bool getaddrport(sockaddr* sa, std::string& hostname, std::string& servname) const
    {
        assert(sa->sa_family == AF_INET || sa->sa_family == AF_INET6);

        int ret = netdb::getnameinfo(sa, hostname, servname);
        return ret == 0;
    }
};

class socket_datagram
{
private:
    int m_fd;

public:
    explicit socket_datagram(int sockfd = -1) : m_fd(sockfd)
    { }

    ~socket_datagram()
    {
        m_fd = -1;
    }

    int close()
    {
        int ret = 0;
        if (m_fd > 0)
        {
            ret = ::close(m_fd);
            m_fd = -1;
        }
        return ret;
    }

    bool is_valid() const
    {
        return m_fd >= 0;
    }

    int sockfd() const
    {
        return m_fd;
    }

//  ssize_t sendto(void* buf, size_t nbytes) const
//  {
//      assert(m_fd >= 0);
//      return ::sendto(m_fd, buf, nbytes, 0, (struct sockaddr *)&m_sockaddr, m_addrlen);
//  }

//  ssize_t recvfrom(void* buf, size_t nbytes, struct sockaddr* from, socklen_t* fromlen) const
//  {
//      assert(m_fd >= 0);
//      return ::recvfrom(m_fd, buf, nbytes, 0, from, fromlen);
//  }
};

class tcp_connector
{
public:
    int connect(const std::string& hostname, const std::string& servname) const
    {
        assert(!hostname.empty());
        assert(!servname.empty());

        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        int ret = -1;
        struct addrinfo* res = NULL;
        if ((ret = ::getaddrinfo(hostname.c_str(), servname.c_str(), &hints, &res)) != 0)
        {
            fprintf(stderr, "tcp connect error for %s, %s: %s\n",
                    hostname.c_str(), servname.c_str(), gai_strerror(ret));
            return -1;
        }
        // call freeaddrinfo automatically
        auto_delete<struct addrinfo, free_functor> aai(res);

        int fd = -1;
        do
        {
            fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (fd < 0)
            {
                continue;
            }

            if ((ret = ::connect(fd, res->ai_addr, res->ai_addrlen)) < 0)
            {
                ::close(fd);
                fd = -1;
                continue;
            }
        } while (fd == -1 && (res = res->ai_next) != NULL);

        return fd;
    }

    int connect(const std::string& hostname, in_port_t port) const
    {
        in_addr sin_addr;
        netdb::inet_pton(hostname, sin_addr);
        return this->connect(sin_addr.s_addr, htons(port));

    //  in_addr_t s_addr = stdx::stdx_inet_addr(hostname);
    //  return this->connect(s_addr, htons(port));
    }

    // addr and port both in network byte order
    int connect(in_addr_t addr, in_port_t port) const
    {
        int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd < 0)
            return -1;

        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = addr;
        servaddr.sin_port = port;

        int ret = ::connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        if (ret < 0)
        {
            ::close(sockfd);
            sockfd = -1;
        }

        return sockfd;
    }
};

class tcp_acceptor
{
private:
    int m_fd;

public:
    tcp_acceptor() : m_fd(-1)
    { }

    int listen(in_port_t port)
    {
        struct sockaddr_in sin;

        int fd = ::socket(AF_INET, SOCK_STREAM, 0);

        bzero(&sin, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
        sin.sin_port = htons(port);

        const int on = 1;
        int ret = ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        if (ret < 0)
        {
            perror("setsockopt");
            ::close(fd);
            return -1;
        }

        if (::bind(fd, (sockaddr *)&sin, sizeof(sin))< 0)
        {
            perror("bind");
            ::close(fd);
            return -1;
        }

        if (::listen(fd, 5) == -1)
        {
            perror("listen");
            ::close(fd);
            return -1;
        }

        m_fd = fd;
        return fd;
    }

    int listen(const std::string& hostname, const std::string& servname, socklen_t* addrlenp)
    {
        assert(m_fd == -1);

        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_flags = AI_PASSIVE;
        hints.ai_family = AF_INET;//AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        int ret = -1;
        struct addrinfo* res = NULL;
        if (hostname.empty())
            ret = ::getaddrinfo(NULL, servname.c_str(), &hints, &res);
        else
            ret = ::getaddrinfo(hostname.c_str(), servname.c_str(), &hints, &res);
        if (ret != 0)
        {
            fprintf(stderr, "tcp_listen error for %s, %s: %s",
                    hostname.c_str(), servname.c_str(), gai_strerror(ret));
            return -1;
        }
        auto_delete<struct addrinfo, free_functor> aai(res);

        do
        {
            m_fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (m_fd < 0)
            {
                continue;
            }

            const int on = 1;
            ret = ::setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
            if (ret < 0)
            {
                ::close(m_fd);
                m_fd = -1;
                continue;
            }

            if (::bind(m_fd, res->ai_addr, res->ai_addrlen) < 0)
            {
                ::close(m_fd);
                m_fd = -1;
                continue;
            }
        } while (m_fd == -1 && (res = res->ai_next) != NULL);

        if (m_fd == -1 || res == NULL)
            return -1;

        if (::listen(m_fd, 50) == -1)
        {
            ::close(m_fd);
            return -1;
        }

        if (addrlenp != NULL)
            *addrlenp = res->ai_addrlen;

        return m_fd;
    }

    int accept(struct sockaddr* addr, socklen_t* addrlen) const
    {
        assert(m_fd >= 0);
        return ::accept(m_fd, addr, addrlen);
    }
};

/*
class udp_connector : public socket_base
{
private:
    socklen_t m_addrlen;
    struct sockaddr_storage m_sockaddr;

public:
    udp_connector(int sockfd = -1) : socket_base(sockfd), m_addrlen(0)
    {
        memset(&m_sockaddr, 0, sizeof(m_sockaddr));
    }

    int connect(const std::string& hostname, const std::string& servname)
    {
        assert(m_fd == -1);
        assert(!hostname.empty());
        assert(!servname.empty());

        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;//AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;

        int ret = -1;
        struct addrinfo* res = NULL;
        if ((ret = ::getaddrinfo(hostname.c_str(), servname.c_str(), &hints, &res)) != 0)
        {
            fprintf(stderr, "ucp connect error for (%s:%s): %s",
                    hostname.c_str(), servname.c_str(), gai_strerror(ret));
            return -1;
        }
        // call freeaddrinfo automatically
        auto_delete<struct addrinfo, free_functor> aai(res);

        do
        {
            m_fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (m_fd < 0)
            {
                continue;
            }

        //  if (::connect(m_fd, res->ai_addr, res->ai_addrlen) < 0)
        //  {
        //      ::close(m_fd);
        //      m_fd = -1;
        //      continue;
        //  }

            memcpy(&m_sockaddr, res->ai_addr, res->ai_addrlen);
            m_addrlen = res->ai_addrlen;

        } while (m_fd == -1 && (res = res->ai_next) != NULL);

        return m_fd;
    }

    ssize_t sendto(void* buf, size_t nbytes) const
    {
        assert(m_fd >= 0);
        return ::sendto(m_fd, buf, nbytes, 0, (struct sockaddr *)&m_sockaddr, m_addrlen);
    }

    ssize_t recvfrom(void* buf, size_t nbytes, struct sockaddr* from, socklen_t* fromlen) const
    {
        assert(m_fd >= 0);
        return ::recvfrom(m_fd, buf, nbytes, 0, from, fromlen);
    }
};

class udp_server : public socket_base
{
private:
    socklen_t m_addrlen;
    struct sockaddr_storage m_sockaddr;

public:
    udp_server(int sockfd = -1) : socket_base(sockfd), m_addrlen(0)
    {
        memset(&m_sockaddr, 0, sizeof(m_sockaddr));
    }

    int listen(const std::string& hostname, const std::string& servname, socklen_t* addrlenp = NULL)
    {
        assert(m_fd == -1);

        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_flags = AI_PASSIVE;
        hints.ai_family = AF_INET;//AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;

        int ret = -1;
        struct addrinfo* res = NULL;
        if (hostname.empty())
            ret = ::getaddrinfo(NULL, servname.c_str(), &hints, &res);
        else
            ret = ::getaddrinfo(hostname.c_str(), servname.c_str(), &hints, &res);
        if (ret != 0)
        {
            fprintf(stderr, "udp_listen error for %s, %s: %s",
                    hostname.c_str(), servname.c_str(), gai_strerror(ret));
            return -1;
        }
        // call freeaddrinfo automatically
        auto_delete<struct addrinfo, free_functor> aai(res);

        do
        {
            m_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (m_fd < 0)
            {
                continue;
            }

            if (bind(m_fd, res->ai_addr, res->ai_addrlen) < 0)
            {
                ::close(m_fd);
                m_fd = -1;
                continue;
            }

            memcpy(&m_sockaddr, res->ai_addr, res->ai_addrlen);
            m_addrlen = res->ai_addrlen;

        } while (m_fd == -1 && (res = res->ai_next) != NULL);

        if (m_fd == -1 || res == NULL)
            return -1;

        if (addrlenp != NULL)
            *addrlenp = res->ai_addrlen;

        return m_fd;
    }
};
*/

class unix_acceptor
{
private:
    int m_fd;

public:
    unix_acceptor() : m_fd(-1)
    { }

    int listen(const std::string& path)
    {
        int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (fd < 0)
            return -1;

        ::unlink(path.c_str());

        struct sockaddr_un sun;
        bzero(&sun, sizeof(sun));
        sun.sun_family = AF_LOCAL;
        strlcpy(sun.sun_path, path.c_str(), sizeof(sun.sun_path));

        if (::bind(fd, (struct sockaddr *)&sun, SUN_LEN(&sun)) == -1)
        {
            perror("bind");
            ::close(fd);
            return -1;
        }

        if (::listen(fd, 5) == -1)
        {
            perror("listen");
            ::close(fd);
            return -1;
        }

        m_fd = fd;
        return fd;
    }

    int accept(struct sockaddr* addr, socklen_t* addrlen) const
    {
        assert(m_fd >= 0);
        return ::accept(m_fd, addr, addrlen);
    }
};

class unix_connector
{
public:
    int connect(const std::string& path) const
    {
        int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (fd < 0)
            return -1;

        struct sockaddr_un sun;
        bzero(&sun, sizeof(sun));
        sun.sun_family = AF_LOCAL;
        strlcpy(sun.sun_path, path.c_str(), sizeof(sun.sun_path));

        int ret = ::connect(fd, (struct sockaddr *)&sun, SUN_LEN(&sun));
        if (ret < 0)
        {
            perror("connect");
            ::close(fd);
            fd = -1;
        }

        return fd;
    }
};


} // namespace stdx


#endif // __STDX_SOCKET_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

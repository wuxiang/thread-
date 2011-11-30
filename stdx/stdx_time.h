#ifndef __STDX_TIME_H
#define __STDX_TIME_H

// C 89 header files
#include <time.h>
#include <assert.h>

namespace stdx {

inline void
millisleep(int ms)
{
    struct timespec ts = { 0, 1000*1000*ms };
    nanosleep(&ts, NULL);
}

class timeinfo
{
private:
    time_t m_val;

public:
    timeinfo()
    {
        m_val = ::time(NULL);
        assert(m_val != -1);
    }

    timeinfo(time_t val) : m_val(val)
    { }

    timeinfo(struct tm *tmptr)
    {
        assert(tmptr != NULL);
        m_val = ::mktime(tmptr);
    }

    struct tm *
    gmtime(struct tm *result) const
    {
        ::gmtime_r(&m_val, result);
        return result;
    }

    struct tm *
    localtime(struct tm *result) const
    {
        ::localtime_r(&m_val, result);
        return result;
    }

    std::string ctime() const
    {
        char buf[128] = { 0 };
        char* pstr = ::ctime_r(&m_val, buf);
        assert(pstr != NULL);
        return std::string(buf);
    }

    std::string asctime() const
    {
        struct tm tm = { 0 };
        struct tm *tmptr = ::localtime_r(&m_val, &tm);
        assert(tmptr != NULL);

        char buf[128] = { 0 };
        char* pstr = ::asctime_r(&tm, buf);
        assert(pstr != NULL);

        return std::string(buf);
    }
};

class tminfo
{
private:
    struct tm m_tm;

public:
    tminfo()
    {
        time_t t = ::time(NULL);
        assert(t != -1);
        struct tm *tmptr = localtime_r(&t, &m_tm);
        assert(tmptr != NULL);
    }

    tminfo(const time_t *tp)
    {
        assert(tp != NULL);
        struct tm *tmptr = localtime_r(tp, &m_tm);
        assert(tmptr != NULL);
    }

    std::string asctime() const
    {
        return "";
    }

    time_t clocktime() const
    {
        struct tm tm = m_tm;
        return ::mktime(&tm);
    }
};


} // namespace stdx

#endif // __STDX_TIME_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

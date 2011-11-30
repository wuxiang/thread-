#ifndef __STDX_MUTEX_H
#define __STDX_MUTEX_H


// Posix header files
#include <sys/stat.h>
#include <pthread.h>

// C 89 header files
#include <assert.h>
#include <errno.h>
#include <stdexcept>

// stdx header files
#include "stdx/stdx_noncopyable.h"

namespace stdx {

struct adopt_lock_t { };

template<typename _Mutex>
class lock_guard
{
public:
    typedef _Mutex mutex_type;

    explicit lock_guard(mutex_type& __m) : _M_device(__m)
    { _M_device.lock(); }

    lock_guard(mutex_type& __m, adopt_lock_t __a) : _M_device(__m)
    { _M_device.lock(); }

    ~lock_guard()
    { _M_device.unlock(); }

    explicit lock_guard(const lock_guard&);
    lock_guard& operator=(const lock_guard&);

private:
    mutex_type&  _M_device;
};

class mutex: private noncopyable
{
private:
    pthread_mutex_t m;

public:
    mutex()
    {
        int const res=pthread_mutex_init(&m, NULL);
        if (res)
        {
            throw std::runtime_error("pthread_mutex_init");
        //  throw thread_resource_error();
        }
    }
    ~mutex()
    {
        int ret = pthread_mutex_destroy(&m);
        assert(ret == 0);
    }
    
    void lock()
    {
        pthread_mutex_lock(&m);
    }

    void unlock()
    {
        pthread_mutex_unlock(&m);
    }
    
    bool try_lock()
    {
        int const res=pthread_mutex_trylock(&m);
        assert(!res || res==EBUSY);
        return !res;
    }

    typedef pthread_mutex_t* native_handle_type;
    native_handle_type native_handle()
    {
        return &m;
    }
};

class condition_variable
{
private:
    pthread_cond_t m_cond;

public:
    condition_variable()
    {
        int ret = pthread_cond_init(&m_cond, NULL);
        if (ret != 0)
        {
            throw std::runtime_error("pthread_cond_init");
        }
    }

    ~condition_variable()
    {
        int ret = pthread_cond_destroy(&m_cond);
        assert (ret == 0);
    }

    void notify_one()
    {
        int ret = pthread_cond_signal(&m_cond);
        assert (ret == 0);
    }

    void notify_all()
    {
        int ret = pthread_cond_broadcast(&m_cond);
        assert (ret == 0);
    }

    void wait(mutex& mtx)
    {
        int ret = pthread_cond_wait(&m_cond, mtx.native_handle());
        assert (ret == 0);
    }
};

} // namespace stdx

#endif // __STDX_MUTEX_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

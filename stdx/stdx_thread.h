#ifndef __STDX_THRPOOL_H
#define __STDX_THRPOOL_H

// Posix header files
#include <pthread.h>

// C++ 98 head file
#include <list>
#include <iostream>

// stdx header files
#include "stdx/stdx_noncopyable.h"
#include "stdx/stdx_task.h"
#include "stdx/stdx_string.h"


namespace stdx {

struct thread_data_base : public stdx::noncopyable
{
    thread_data_base() : m_join_started(false), m_joined(false) {}
    virtual ~thread_data_base() {}
    virtual void run() = 0;

    pthread_t m_tid;
    bool m_join_started;
    bool m_joined;
    stdx::mutex m_data_mutex;
};

template<typename F>
struct thread_data : public thread_data_base
{
public:
    thread_data(F f_): f(f_) {}
    void run() { f(); }
private:
    F f;
};

typedef thread_data_base*   thread_data_ptr;

} // namespace stdx


namespace {

    extern "C"
    {
        inline void* thread_proxy(void* param)
        {
            stdx::thread_data_base* pdata = static_cast<stdx::thread_data_base*>(param);
            pdata->run();
            return 0;
        }
    }

} // anonymouse namespace


namespace stdx {

class thread : public stdx::noncopyable
{
private:
    thread_data_ptr m_info;

    void start_thread()
    {
        int const res = pthread_create(&m_info->m_tid, 0, &thread_proxy, m_info);
        if (res != 0)
        {
            throw std::runtime_error(stdx::stdx_strerror("duke::thread::pthread_create: "));
        }
    }

    inline thread_data_ptr get_thread_info() const
    {
        return m_info;
    }

    template <typename F>
    static inline thread_data_ptr make_thread_info(F f)
    {
        return thread_data_ptr(new thread_data<F>(f));
    }

public:
    thread() : m_info(NULL)
    {}

    template <typename F>
    thread(F f) : m_info(make_thread_info(f))
    {
        start_thread();
    }

    ~thread()
    {
        detach();
    }

    void join()
    {
        stdx::lock_guard<stdx::mutex> mtx(m_info->m_data_mutex);
        thread_data_ptr const info = get_thread_info();
        if (!info->m_join_started)
        {
            info->m_join_started = true;
            info->m_joined = true;
            void* result = 0;
            pthread_join(info->m_tid, &result);
        }
    }

    void detach()
    {
        stdx::lock_guard<stdx::mutex> mtx(m_info->m_data_mutex);
        thread_data_ptr const info = get_thread_info();
        if (!info->m_join_started)
        {
            pthread_detach(info->m_tid);
            info->m_join_started = true;
            info->m_joined = true;
        }
    }

    int cancel() const
    {
        thread_data_ptr const info = get_thread_info();
        return pthread_cancel(info->m_tid);
    }
};

}


namespace stdx {

struct thread_pool_data
{
    thread_pool_data() : m_running(true)
    { }
    volatile bool m_running;
    stdx::task_pool m_pool;
    stdx::mutex m_mutex;
    stdx::condition_variable m_cond;
};

} // namespace stdx

namespace
{
    extern "C"
    {
        inline void thread_cleanup_routine(void* arg)
        {
            stdx::thread_pool_data* pdata = static_cast<stdx::thread_pool_data*>(arg);
            stdx::mutex& mtx = pdata->m_mutex;
            mtx.unlock();
        }

        inline void* thread_pool_routine(void* arg)
        {
            pthread_cleanup_push(thread_cleanup_routine, arg);

            stdx::thread_pool_data* pdata = static_cast<stdx::thread_pool_data*>(arg);
            stdx::task_pool& pool = pdata->m_pool;
            stdx::condition_variable& cond = pdata->m_cond;
            stdx::mutex& mtx = pdata->m_mutex;

            while (pdata->m_running)
            {
                stdx::task_base* ptask = pool.pop();
                if (ptask != NULL)
                {
                    ptask->run();
                    delete ptask;
                }
                else
                {
                    mtx.lock();
                    cond.wait(mtx);
                    mtx.unlock();
                }
            }

            pthread_cleanup_pop(0);

            return 0;
        }
    }
}

namespace stdx {

class thread_pool
{
private:
    typedef std::list<pthread_t>    threadid_list;

    thread_pool_data m_data;
    std::list<pthread_t> m_tids;

public:
    thread_pool(int num, bool bdetach = true)
    {
        while (num-- > 0)
        {
            pthread_t pid;
            int ret = pthread_create(&pid, NULL, thread_pool_routine, &m_data);
            assert(ret == 0);
            m_tids.push_back(pid);
        }
    }

    template <typename F>
    void push(F f)
    {
        m_data.m_pool.push(f);
        m_data.m_cond.notify_one();
    }

    void notify()
    {
        m_data.m_running = false;
        m_data.m_cond.notify_all();
    }

    void cancel()
    {
        for (threadid_list::iterator it = m_tids.begin(); it != m_tids.end(); ++it)
        {
            pthread_cancel(*it);
        }
    }

    void join()
    {
        for (threadid_list::iterator it = m_tids.begin(); it != m_tids.end(); ++it)
        {
            void* pvalue;
            pthread_join(*it, &pvalue);
        }
    }

    void detach()
    {
        for (threadid_list::iterator it = m_tids.begin(); it != m_tids.end(); ++it)
        {
            pthread_detach(*it);
        }
    }
};

} // namespace stdx


#endif // __STDX_THRPOOL_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

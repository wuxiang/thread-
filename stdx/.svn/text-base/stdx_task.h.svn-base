#ifndef __STDX_TASK_H
#define __STDX_TASK_H

// Posix header files
#include <pthread.h>

// C++ 98 head file
#include <list>
#include <queue>

// stdx header files
#include "stdx/stdx_mutex.h"


namespace stdx {

class task_base
{
public:
    virtual ~task_base() {}
    virtual void run()=0;
};

template <typename F>
class task : public task_base
{
public:
    task(F f_): f(f_) {}
    void run() { f(); }
private:
    F f;
};

class task_pool
{
private:
    mutex m_mutex;
//  std::list<task_base*> m_tasks;
    std::queue<task_base*> m_tasks;

public:
    template <typename F>
    void push(F f)
    {
        task<F>* t = new task<F>(f);
        lock_guard<mutex> guard(m_mutex);
        m_tasks.push(t);
    }

    task_base* pop()
    {
        lock_guard<mutex> guard(m_mutex);
        task_base* task = NULL;
        if (!m_tasks.empty())
        {
            task = m_tasks.front();
            m_tasks.pop();
        }
        return task;
    }

    bool empty()
    {
        lock_guard<mutex> guard(m_mutex);
        return m_tasks.empty();
    }
};

} // namespace stdx


#endif // __STDX_TASK_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

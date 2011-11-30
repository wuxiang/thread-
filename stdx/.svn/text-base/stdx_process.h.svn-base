#ifndef __STDX_PROCESS_H
#define __STDX_PROCESS_H


// Posix header files
#include <sys/wait.h>
#include <unistd.h>

// C 89 header files
#include <assert.h>
#include <stdlib.h>
#include <setjmp.h>

// C++ 98
//#include <vector>

// stdx header files
#include "stdx/stdx_log.h"
#include "stdx_string.h"


namespace stdx {


class process_env
{
public:
    std::string read(const std::string& name) const
    {
        assert(!name.empty());
        std::string strpair;
        char* val = ::getenv(name.c_str());
        if (val != NULL)
            strpair = val;
        return strpair;
    }

    bool write(const std::string& strpair) const
    {
        assert(strpair.find('=') != std::string::npos);
        const size_t len = strpair.size() + 1;
        char* buf = static_cast<char*>(malloc(len));
        stdx::strlcpy(buf, strpair.c_str(), len);
        int ret = ::putenv(buf);
        return (ret == 0);
    }

    bool write(const std::string& name, const std::string& value,
            bool rewrite = true) const
    {
        assert(!name.empty() && !value.empty());
        int ret = ::setenv(name.c_str(), value.c_str(), rewrite ? 1 : 0);
        return (ret == 0);
    }

    bool erase(const std::string& name)
    {
        assert(!name.empty());
        int ret = ::unsetenv(name.c_str());
        return (ret == 0);
    }
};


class process
{
private:
    pid_t m_pid;

public:
    process() : m_pid(-1)
    { }

    pid_t fork()
    {
        m_pid = ::fork();
        return m_pid;
    }

    bool is_child() const
    {
        return m_pid == 0;
    }

    bool is_parent() const
    {
        return m_pid > 0;
    }

    int wait_child(int* statloc, int options = 0) const
    {
        assert(this->is_parent());
        return ::waitpid(m_pid, statloc, options);
    }

    int wait_parent(int* statloc, int options = 0) const
    {
        assert(this->is_child());
        return ::waitpid(::getpid(), statloc, options);
    }

    int exec()
    {
        return 0;
    }
};


// The setjmp(3) manual page on one system states that variables stored in memory
// will have values as of the time of the longjmp, whereas variables in the CPU
// and floating-point registers are restored to their values when setjmp was called.

void print_exit_status(int status, const std::string& prefix = "")
{
    if (WIFEXITED(status))
    {
        LOG_INFO(prefix << "normal termination, exit status = " << WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        std::string strcore;
#ifdef  WCOREDUMP
        strcore = WCOREDUMP(status) ? " (core file generated)" : "";
#endif
        LOG_ERROR(prefix << "abnormal termination, signal number = " << WTERMSIG(status) << strcore);
    }
    else if (WIFSTOPPED(status))
    {
        LOG_INFO(prefix << "child stopped, signal number = " << WSTOPSIG(status));
    }
}


} // namespace stdx


#endif // __STDX_PROCESS_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

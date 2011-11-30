#ifndef __STDX_IPC_H
#define __STDX_IPC_H


// Posix header files
#include <sys/wait.h>
#include <unistd.h>

// C 89 header files
#include <signal.h>

// stdx header files
#include "stdx/stdx_log.h"


namespace stdx {


class signal_sync
{
private:
    static volatile sig_atomic_t m_sigflag = 0; /* set nonzero by sig handler */
    sigset_t newmask, oldmask, zeromask;

public:
    static void sig_usr(int signo)   /* one signal handler for SIGUSR1 and SIGUSR2 */
    {
        m_sigflag = 1;
    }
};


void
TELL_WAIT(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR2) error");
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);

    /*
     * Block SIGUSR1 and SIGUSR2, and save current signal mask.
     */
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");
}

void
TELL_PARENT(pid_t pid)
{
    kill(pid, SIGUSR2);              /* tell parent we're done */
}

void
WAIT_PARENT(void)
{
    while (sigflag == 0)
        sigsuspend(&zeromask);   /* and wait for parent */
    sigflag = 0;

    /*
     * Reset signal mask to original value.
     */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");
}

void
TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR1);             /* tell child we're done */
}

void
WAIT_CHILD(void)
{
    while (sigflag == 0)
        sigsuspend(&zeromask);  /* and wait for child */
    sigflag = 0;

    /*
     * Reset signal mask to original value.
     */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");
}


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
        strlcpy(buf, strpair.c_str(), len);
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


#endif // __STDX_IPC_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

#ifndef __STDX_TTYMODE_H
#define __STDX_TTYMODE_H


// Posix header files
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

// C 89 header files
#include <assert.h>
#include <errno.h>

// C++ 98 header files
#include <stdexcept>


namespace stdx {


class ttymode
{
public:
    enum ttystate { RESET, RAW, CBREAK };
public:
    ttymode(int fd) : m_fd(fd), m_state(RESET)
    {
        assert(fd >= 0);
        if (tcgetattr(fd, &m_mode) < 0)
            throw std::runtime_error("ttymode ctor()");
    }

    ~ttymode()
    {
        reset();
    }

    int reset()
    {
        if (m_state == RESET)
            return 0;
        if (tcsetattr(m_fd, TCSAFLUSH, &m_mode) < 0)
            return -1;
        m_state = RESET;
        return 0;
    }

    int cbreak()
    {
        if (m_state != RESET)
        {
            errno = EINVAL;
            return -1;
        }

        struct termios mode = m_mode;
        mode.c_lflag &= ~(ECHO | ICANON);
        mode.c_cc[VMIN] = 1;
        mode.c_cc[VTIME] = 0;
        if (tcsetattr(m_fd, TCSAFLUSH, &mode) < 0)
            return -1;

        if (tcgetattr(m_fd, &mode) < 0)
        {
            int err = errno;
            reset();
            errno = err;
            return -1;
        }

        if ((mode.c_lflag & (ECHO | ICANON)) || mode.c_cc[VMIN] != 1 || mode.c_cc[VTIME] != 0)
        {
            reset();
            errno = EINVAL;
            return -1;
        }

        m_state = CBREAK;

        return 0;
    }

private:
    int m_fd;
    ttystate m_state;
    termios m_mode;
};


} // namespace stdx


#endif // __STDX_TTYMODE_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

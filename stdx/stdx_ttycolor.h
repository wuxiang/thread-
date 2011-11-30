#ifndef __STDX_TTYCOLOR_H
#define __STDX_TTYCOLOR_H


// Posix header files
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

// C 89 header files
#include <assert.h>
#include <errno.h>

// C++ 98 header files
#include <string>


namespace stdx {

enum _Ios_colorflags
{
    _S_black        = 0x00,
    _S_blue         = 0x01,
    _S_green        = 0x02,
    _S_darkblue     = 0x03,
    _S_red          = 0x04,
    _S_purple       = 0x05,
    _S_yellow       = 0x06,
    _S_white        = 0x07,
    _S_grey         = 0x08,
    _S_lightblue    = 0x09,
    _S_lightgreen   = 0x0A,
    _S_lightgreen2  = 0x0B,
    _S_lightred     = 0x0C,
    _S_lightpurple  = 0x0D,
    _S_lightyellow  = 0x0E,
    _S_lightwhite   = 0x0F,
};

static const std::string _Text_endcolor     = "\033[0m";
static const std::string _Text_black        = "\033[22;30m";
static const std::string _Text_red          = "\033[22;31m";
static const std::string _Text_green        = "\033[22;32m";
static const std::string _Text_brown        = "\033[22;33m";
static const std::string _Text_blue         = "\033[22;34m";
static const std::string _Text_magenta      = "\033[22;35m";
static const std::string _Text_cyan         = "\033[22;36m";
static const std::string _Text_gran         = "\033[22;37m";
static const std::string _Text_dark         = "\033[01;30m";
static const std::string _Text_lightred     = "\033[01;31m";
static const std::string _Text_lightgreen   = "\033[01;32m";
static const std::string _Text_yellow       = "\033[01;33m";
static const std::string _Text_lightblue    = "\033[01;34m";
static const std::string _Text_lightmagenta = "\033[01;35m";
static const std::string _Text_lightcyan    = "\033[01;36m";
static const std::string _Text_white        = "\033[01;37m";
static const std::string _Back_black        = "\033[22;40m";
static const std::string _Back_red          = "\033[22;41m";
static const std::string _Back_green        = "\033[22;42m";
static const std::string _Back_brown        = "\033[22;43m";
static const std::string _Back_blue         = "\033[22;44m";
static const std::string _Back_magenta      = "\033[22;45m";
static const std::string _Back_cyan         = "\033[22;46m";
static const std::string _Back_gran         = "\033[22;47m";
static const std::string _Back_dark         = "\033[01;40m";
static const std::string _Back_lightred     = "\033[01;41m";
static const std::string _Back_lightgreen   = "\033[01;42m";
static const std::string _Back_yellow       = "\033[01;43m";
static const std::string _Back_lightblue    = "\033[01;44m";
static const std::string _Back_lightmagenta = "\033[01;45m";
static const std::string _Back_lightcyan    = "\033[01;46m";
static const std::string _Back_white        = "\033[01;47m";


struct _Log_color { std::string m_color; };

inline _Log_color
setcolor(const std::string& clr)
{
    _Log_color __x;
    __x.m_color = clr;
    return __x;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
operator<<(std::basic_ostream<_CharT, _Traits>& __os, _Log_color __f)
{
    __os << __f.m_color << std::flush;
    return __os;
}


template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
endcolor(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_endcolor << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
black(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_black << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
red(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_red << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
green(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_green << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
brown(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_brown << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
blue(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_blue << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
magenta(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_magenta << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
cyan(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_cyan << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
gran(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_gran << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
dark(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_dark << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
lightred(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_lightred << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
lightgreen(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_lightgreen << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
yellow(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_yellow << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
lightblue(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_lightblue << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
lightmagenta(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_lightmagenta << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
lightcyan(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_lightcyan << std::flush;
    return __os;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
white(std::basic_ostream<_CharT, _Traits>& __os)
{
    __os << _Text_white << std::flush;
    return __os;
}

/*
template <typename _CharT, typename _Traits>
struct _Log_linecolor
{
    _Log_linecolor(std::basic_ostream<_CharT, _Traits>& __os) : m_os(__os)
    { }

    ~_Log_linecolor()
    {
        (*m_os) << disable_color() << std::flush;
    }

    std::basic_ostream<_CharT, _Traits>* m_os;
    std::string m_color;
};

inline _Log_linecolor
setlinecolor(const std::string& clr)
{
    _Log_color __x;
    __x.m_color = clr;
    return __x;
}

template <typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits>&
operator<<(std::basic_ostream<_CharT, _Traits>& __os, _Log_linecolor<_CharT, _Traits> __f)
{
    __f.m_os = & __os;
    __os << __f.m_color << std::flush;
    return __os;
}
*/


} // namespace stdx


#endif // __STDX_TTYCOLOR_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

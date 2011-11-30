#ifndef __STDX_SYSINFO_H
#define __STDX_SYSINFO_H


// Posix header files
#include <unistd.h>
#include <pwd.h>
#include <netdb.h>

// C++ 98
//#include <vector>

// stdx header files
//#include "stdx_memory.h"
//#include "stdx_string.h"


namespace stdx {

// forward iterator
class pwlist
{
    pwlist()
    {
        ::setpwent();
    }

    ~pwlist()
    {
        ::endpwent();
    }
};


} // namespace stdx


#endif // __STDX_SYSINFO_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

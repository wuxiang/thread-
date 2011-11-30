#ifndef __STDX_FS_H
#define __STDX_FS_H


// Posix header files
#include <sys/stat.h>

// C 89 header files
#include <assert.h>
#include <errno.h>

// C++ 98 header files
#include <string>


namespace stdx {

class filesystem
{
private:
    int m_fd;

public:
    filesystem(int fd = -1) : m_fd(fd) {}

    filesystem(const std::string& name, int oflag, mode_t mode)
    {
        this->open(name.c_str(), oflag, mode);
    }

    ~filesystem()
    {
        this->close();
    }

    int open(const std::string& name, int oflag, mode_t mode)
    {
        m_fd = ::open(name.c_str(), oflag, mode);
        return m_fd;
    }

    int close()
    {
        int ret = -1;
        if (m_fd >= 0)
        {
            m_fd = -1;
            ret = ::close(m_fd);
        }
        return ret;
    }

    off_t lseek() const
    {
        return ::lseek(m_fd, 0, SEEK_CUR); // SEEK_SET, SEEK_CUR, SEEK_END
    }

    off_t lseek(off_t offset, int whence) const
    {
        return ::lseek(m_fd, offset, whence);
    }

    ssize_t read(void *buf, size_t nbytes) const
    {
        return ::read(m_fd, buf, nbytes);
    }
    
    ssize_t write(const void *buf, size_t nbytes) const
    {
        return ::write(m_fd, buf, nbytes);
    }

    ssize_t pread(void *buf, size_t nbytes, off_t offset) const
    {
        return ::pread(m_fd, buf, nbytes, offset);
    }

    ssize_t pwrite(const void *buf, size_t nbytes, off_t offset) const
    {
        return ::pwrite(m_fd, buf, nbytes, offset);
    }

    filesystem dup() const
    {
        int fd = ::dup(m_fd);
        return filesystem(fd);
    }

    int fsync() const
    {
        return ::fsync(m_fd);
    }

//  int fdatasync() const
//  {
//      return ::fdatasync(m_fd);
//  }

    static void sync(void)
    {
        return ::sync();
    }

    bool exist() const
    {
        struct stat st;
        return (::fstat(m_fd, &st) == 0 && S_ISREG(st.st_mode));
    }

    static bool exist(const std::string& name)
    {
        assert(!name.empty());
        struct stat st;
        return (::stat(name.c_str(), &st) == 0 && S_ISREG(st.st_mode));
    }

    static bool mkdir(const std::string& dir)
    {
        assert(!dir.empty());
        bool result = false;
        struct stat sb;
        if (!dir.empty() && ::stat(dir.c_str(), &sb) == -1)
        {
            if (::mkdir(dir.c_str(), S_IRWXU) == -1)
                result = false;
            else
                result = true;
        }
        return result;
    }
};

inline bool
stdx_fileexist(const std::string& name)
{
    return filesystem::exist(name);
}

inline bool
stdx_mkdir(const std::string& dir)
{
    return filesystem::mkdir(dir);
}


#define DUKE_PAGE_SIZE_DEFAULT 4096

inline uint32_t
stdx_getpagesize(void)
{
#ifdef PAGE_SIZE
    uint32_t pagesize = PAGE_SIZE;
#else
    uint32_t pagesize = 0;
#endif
    if (pagesize == 0)
    {
        errno = 0;
        if ((pagesize = sysconf(_SC_PAGE_SIZE)) < 0)
        {
            if (errno == 0)
                pagesize = DUKE_PAGE_SIZE_DEFAULT;
            else
                assert(!"sysconf error for _SC_PAGE_SIZE");
        }
    }
    return pagesize;
}

} // namespace stdx

#endif // __STDX_FS_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

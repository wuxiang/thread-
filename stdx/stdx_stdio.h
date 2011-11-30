#ifndef __STDX_STDIO_H
#define __STDX_STDIO_H


// C 89 header files
#include <stdarg.h>
#include <stdio.h>

// C++ 98 header files
#include <string>


namespace stdx {


// When a file is opened for reading and writing (the plus sign in the type),
// the following restrictions apply.
// 1. Output cannot be directly followed by input without an intervening
//    fflush, fseek, fsetpos,or rewind.
// 2. Input cannot be directly followed by output without an intervening
//    fseek, fsetpos,or rewind, or an input operation that encounters an end of file.

class stdio
{
private:
    FILE* m_fp;

public:
    stdio(const std::string& name, const std::string& type)
    {
        this->fopen(name, type);
    }

    ~stdio()
    {
        this->fclose();
    }

    // fopen is from ISO C
    FILE* fopen(const std::string& name, const std::string& type)
    {
        return (m_fp = ::fopen(name.c_str(), type.c_str()));
    }

    // freopen is from ISO C
    FILE* freopen(const std::string& name, const std::string& type, FILE* fp)
    {
        return (m_fp = ::freopen(name.c_str(), type.c_str(), fp));
    }

    // fdopen is from POSIX.1
    FILE* fdopen(int fd, const std::string& type)
    {
        return (m_fp = ::fdopen(fd, type.c_str()));
    }

    int fclose()
    {
        int ret = EOF;
        if (m_fp != NULL)
        {
            ret = ::fclose(m_fp);
            m_fp = NULL;
        }
        return ret;
    }

    // setbuf/setvbuf must be called after the stream has been opened
    // (obviously, since each requires a valid file pointer as its first argument)
    // but before any other operation is performed on the stream.
    // To enable buffering, buf must point to a buffer of length BUFSIZ,
    // a constant defined in <stdio.h>.
    void setbuf(char* buf) const
    {
        return ::setbuf(m_fp, buf);
    }

    // If the stream is buffered and buf is NULL, the standard I/O library will
    // automatically allocate its own buffer of the appropriate size for the stream.
    int setvbuf(char* buf, int mode, size_t size) const
    {
        return ::setvbuf(m_fp, buf, mode, size);
    }

    int setfullbuf(char* buf, size_t size) const
    {
        return this->setvbuf(buf, _IOFBF, size);
    }

    int setlinebuf(char* buf, size_t size) const
    {
        return this->setvbuf(buf, _IOLBF, size);
    }

    int setunbuf() const
    {
        return this->setvbuf(NULL, _IONBF, 0);
    }

    int fflush() const
    {
        return ::fflush(m_fp);
    }

    int ferror() const
    {
        return ::ferror(m_fp);
    }

    int feof() const
    {
        return ::feof(m_fp);
    }

    void clearerr() const
    {
        return ::clearerr(m_fp);
    }

    int getc() const
    {
        return ::getc(m_fp);
    }

    int fgetc() const
    {
        return ::fgetc(m_fp);
    }

    static int getchar()
    {
        return ::getchar();
    }

    int ungetc(int c)
    {
        return ::ungetc(c, m_fp);
    }

//  int putc(int c) const
//  {
//      return ::putc(c, m_fp);
//  }

    int fputc(int c) const
    {
        return ::fputc(c, m_fp);
    }

    static int putchar(int c)
    {
        return ::putchar(c);
    }

    // buf includes the next newline
    char* fgets(char* buf, int n) const
    {
        return ::fgets(buf, n, m_fp);
    }

    // gets deletes a newline, fgets keeps it
    static char* gets(char* buf)
    {
        return ::gets(buf);
    }

    int fputs(const char* buf) const
    {
        return ::fputs(buf, m_fp);
    }

    // puts adds a newline, fputs doesn't
    static int puts(const char* buf)
    {
        return ::puts(buf);
    }

    size_t fread(void* ptr, size_t size, size_t nobj) const
    {
        return ::fread(ptr, size, nobj, m_fp);
    }

    size_t fwrite(const void* ptr, size_t size, size_t nobj) const
    {
        return ::fwrite(ptr, size, nobj, m_fp);
    }

    // Returns: current file position indicator if OK, -1L on error
    long ftell() const
    {
        return ::ftell(m_fp);
    }

    // Returns: 0 if OK, nonzero on error
    int fseek(FILE *fp, long offset, int whence) const
    {
        return ::fseek(m_fp, offset, whence);
    }

    void rewind() const
    {
        return ::rewind(m_fp);
    }

    // ftello and fseeko were introduced in the Single UNIX Specification
    // to allow for file offsets that might not fit in a long integer.
    //
    // Returns: current file position indicator if OK, (off_t)1 on error
    off_t ftello() const
    {
        return ::ftello(m_fp);
    }

    // Returns: 0 if OK, nonzero on error
    int fseeko(FILE *fp, off_t offset, int whence) const
    {
        return ::fseeko(m_fp, offset, whence);
    }

    // Both return: 0 if OK, nonzero on error
    int fgetpos(fpos_t *pos) const
    {
        return ::fgetpos(m_fp, pos);
    }

    // Both return: 0 if OK, nonzero on error
    int fsetpos(const fpos_t *pos) const
    {
        return ::fsetpos(m_fp, pos);
    }

    // Both return: number of characters output if OK, negative value if output error
    int printf(const char *format, ...) const
    {
        va_list ap;
        va_start(ap, format);
        int ret = ::vprintf(format, ap);
        va_end(ap);
        return ret;
    }

    // Both return: number of characters output if OK, negative value if output error
    static int fprintf(FILE *fp, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        int ret = ::vfprintf(fp, format, ap);
        va_end(ap);
        return ret;
    }

    // Both return: number of characters stored in array if OK, negative value if encoding error
    int sprintf(char *buf, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        int ret = ::vsprintf(buf, format, ap);
        va_end(ap);
        return ret;
    }

    // Both return: number of characters stored in array if OK, negative value if encoding error
    int snprintf(char *buf, size_t n, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        int ret = ::vsnprintf(buf, n, format, ap);
        va_end(ap);
        return ret;
    }

    // Both return: number of characters output if OK, negative value if output error
    int vprintf(const char *format, va_list arg) const
    {
        return ::vprintf(format, arg);
    }

    // Both return: number of characters output if OK, negative value if output error
    static int vfprintf(FILE *fp, const char *format, va_list arg)
    {
        return ::vfprintf(fp, format, arg);
    }

    // Both return: number of characters stored in array if OK, negative value if encoding error
    int vsprintf(char *buf, const char *format, va_list arg) const
    {
        return ::vsprintf(buf, format, arg);
    }

    // Both return: number of characters stored in array if OK, negative value if encoding error
    int vsnprintf(char *buf, size_t n, const char *format, va_list arg) const
    {
        return ::vsnprintf(buf, n, format, arg);
    }

    // All six return: number of input items assigned,
    // EOF if input error or end of file before any conversion

    static int scanf(const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        int ret = ::vscanf(format, ap);
        va_end(ap);
        return ret;
    }

    int fscanf(const char *format, ...) const
    {
        va_list ap;
        va_start(ap, format);
        int ret = ::vfscanf(m_fp, format, ap);
        va_end(ap);
        return ret;
    }

    static int sscanf(const char *buf, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        int ret = ::vsscanf(buf, format, ap);
        va_end(ap);
        return ret;
    }

    static int vscanf(const char *format, va_list arg)
    {
        return ::vscanf(format, arg);
    }

    int vfscanf(const char *format, va_list arg) const
    {
        return ::vfscanf(m_fp, format, arg);
    }

    static int vsscanf(const char *buf, const char *format, va_list arg)
    {
        return ::vsscanf(buf, format, arg);
    }

//  int fileno() const
//  {
//      return ::fileno(m_fp);
//  }

    // The ISO C standard
    static char *tmpnam(char *ptr)
    {
        return ::tmpnam(ptr);
    }

    // The ISO C standard
    static FILE *tmpfile(void)
    {
        return ::tmpfile();
    }

    // XSI, Returns: pointer to unique pathname
    static char *tempnam(const char *directory, const char *prefix)
    {
        return ::tempnam(directory, prefix);
    }

    // #include <stdlib.h>
    // XSI, Returns: file descriptor if OK, 1 on error
    // Unlike tmpfile, the temporary file created by mkstemp is not removed automatically for us.
    static int mkstemp(char *fmt)
    {
        return ::mkstemp(fmt);
    }
};


} // namespace stdx

#endif // __STDX_STDIO_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

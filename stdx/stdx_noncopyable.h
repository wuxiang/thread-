#ifndef __STDX_NONCOPYABLE_H
#define __STDX_NONCOPYABLE_H

namespace stdx {

class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

} // namespace stdx

#endif // __STDX_NONCOPYABLE_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

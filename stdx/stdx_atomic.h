#ifndef __STDX_ATOMIC_H
#define __STDX_ATOMIC_H

namespace stdx {

// We use GCC's atomic-buitins to implements Atomic operations
// "In most cases, these builtins are considered a full barrier."

template<typename T>
inline T sync_fetch(const T *pt)
{
   __sync_synchronize();    // full barrier (may be ommited?)
   T tmp = *pt;
   __sync_synchronize();    // full barrier

   return tmp;
}


template<typename T>
inline void sync_set(T *pt, T val)
{
   __sync_synchronize();    // full barrier
   *pt = val;
   __sync_synchronize();    // full barrier (may be ommited?)
}


template <typename T>
T sync_fetch_and_inc(T* pt)
{
    // increase by 1 and return the previous value
    return __sync_fetch_and_add(pt, 1);
}


template <typename T>
T sync_inc_and_fetch(T* pt)
{
    // increase by 1 and return the later value
    return __sync_add_and_fetch(pt, 1);
}

template <typename T>
T sync_fetch_and_add(T* pt, T val)
{
    // increase by val and return the previous value
    return __sync_fetch_and_add(pt, val);
}


template <typename T>
T sync_add_and_fetch(T* pt, T val)
{
    // increase by val and return the later value
    return __sync_add_and_fetch(pt, val);
}

// TODO add support for other atomic operations

} // namespace stdx

#endif // __STDX_ATOMIC_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

#ifndef __NB_STDX_SINGLETON_H
#define __NB_STDX_SINGLETON_H

// Posix header files
#include <pthread.h>

// C++ 98 header files
#include <stdexcept>

// Boost header file
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>

template <typename _Tp>
class mutex_singleton : public boost::noncopyable
{
private:
    static boost::mutex s_mutex_;
    static boost::scoped_ptr<_Tp> s_obj_;

public:
    static _Tp& instance (void)
    {
        if (s_obj_.get() == 0)
        {
            boost::lock_guard<boost::mutex> mtx(s_mutex_);
            if (s_obj_.get() == 0)
                s_obj_.reset(new _Tp);
        }
        return *s_obj_;
    }
};

template <typename _Tp>
boost::mutex
mutex_singleton<_Tp>::s_mutex_;

template <typename _Tp>
//auto_delete<_Tp>
boost::scoped_ptr<_Tp>
mutex_singleton<_Tp>::s_obj_;



//
// THE FOLLOW CLASS IS FROM BOOST LIBRARY
//
//
// The classes below support usage of singletons, including use in program startup/shutdown code, AS LONG AS there is only one thread running before main() begins, and only one thread running after main() exits.
//
// This class is also limited in that it can only provide singleton usage for classes with default constructors.

// _Tp must be: no-throw default constructible and no-throw destructible
template <typename _Tp>
struct boost_singleton : public boost::noncopyable
{
private:
    struct object_creator
    {
        // This constructor does nothing more than ensure that instance()
        //  is called before main() begins, thus creating the static
        //  _Tp object before multithreading race issues can come up.
        object_creator() { boost_singleton<_Tp>::instance(); }
        inline void do_nothing() const { }
    };
    static object_creator s_creator;

public:
    // If, at any point (in user code), boost_singleton<_Tp>::instance()
    //  is called, then the following function is instantiated.
    static _Tp& instance()
    {
        // This is the object that we return a reference to.
        // It is guaranteed to be created before main() begins because of
        //  the next line.
        static _Tp s_obj;

        // The following line does nothing else than force the instantiation
        //  of boost_singleton<_Tp>::create_object, whose constructor is
        //  called before main() begins.
        s_creator.do_nothing();

        return s_obj;
    }
};

template <typename _Tp>
typename boost_singleton<_Tp>::object_creator
boost_singleton<_Tp>::s_creator;



#endif // __NB_STDX_SINGLETON_H

// vim:set tabstop=4 shiftwidth=4 expandtab:

/***********************************
 *
 *write a test for boost::thread ,but it dose not work
 *
 ***********************************/
#define BOOST_DATE_TIME_SOURCE
#define BOOST_THREAD_NO_LIB
#include <iostream>
#include <unistd.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

void func(const char* name, int mokey)
{
	boost::mutex m_mutex;
	boost::mutex::scoped_lock m_lock(m_mutex);
	std::cout << name << ": make money " << mokey << "a day" << std::endl;
}

int main()
{
	boost::thread thread1(boost::bind(func, "tom", 160));
	boost::thread thread2(boost::bind(func, "glen", 500));

	sleep(3);
	thread1.join();
	thread2.join();
	return 0;
}

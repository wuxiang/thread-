/***************************************************
 * write a test case for condition in mutil_thread 
 * proram, and it work well
 * it add by elison.wu
 * *************************************************/
#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <stack>
#include <unistd.h>

boost::mutex io_mu;
template<typename _T>
class buffer
{
	private:
		typedef _T  T;
		boost::mutex m_mutex;
		boost::condition_variable_any m_cond_read;
		boost::condition_variable_any m_cond_write;
		std::stack<T> m_data;
		int m_has_data;
		int m_capacity;

		bool is_full()
		{
			return (m_has_data == m_capacity);
		}

		bool is_empty()
		{
			return (0 == m_has_data);
		}
	public:
		buffer(std::size_t n):m_has_data(0), m_capacity(n)
		{
		}

		void input(T& src)
		{
			{
				boost::mutex::scoped_lock s_lock(m_mutex);
				while(this->is_full())
				{
					//prevent io_mu to be dead lock
					{
						boost::mutex::scoped_lock io_lock(io_mu);
						std::cout << "buffer is full..............." << std::endl;
					}
					m_cond_write.wait(m_mutex, !boost::bind(&buffer::is_full, this));
					//m_cond_write.wait(m_mutex);
				}
				m_data.push(src);
				++m_has_data;
			}
			m_cond_read.notify_one();
		}

		void get(T& value)
		{
			{
				boost::mutex::scoped_lock s_lock(m_mutex);
				while(this->is_empty())
				{
					{
						boost::mutex::scoped_lock io_lock(io_mu);
						std::cout << "buffer is empty.............." << std::endl;
					}
					m_cond_read.wait(m_mutex, !boost::bind(&buffer::is_empty, this));
					//m_cond_read.wait(m_mutex);
				}
				value = m_data.top();
				m_data.pop();
				--m_has_data;
			}
			m_cond_write.notify_one();
		}
};

buffer<int> tmp(10);


void producer(void)
{
	for(int i = 0; ; ++i)
	{
		{
			boost::mutex::scoped_lock lk(io_mu);
			std::cout << "put num: " << i << std::endl;
		}
		tmp.input(i);
		sleep(1);
	}
}


void cumster(void)
{
	int x = 0;
	while(1)
	{
		tmp.get(x);
		{
			boost::mutex::scoped_lock lk(io_mu);
			std::cout << "get num: " << x << std::endl;
		}
		sleep(1);
	}
}

int main()
{
	boost::thread thread1(boost::bind(producer));
	boost::thread thread2(boost::bind(cumster));
	boost::thread thread3(boost::bind(cumster));

	thread1.join();
	thread2.join();
	thread3.join();
	return 0;
}

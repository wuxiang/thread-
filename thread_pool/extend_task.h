/***********************************************************************************
 * *
 * * Copyright 2011.9.10
 * * author elison.wu
 * *
 * ********************************************************************************/
#ifndef _BOOST_TASK_H
#define _BOOST_TASK_H

#include <boost/thread.hpp>
#include <queue>
#include <boost/shared_ptr.hpp>

namespace extend
{
	class task_base
	{
		public:
			task_base()
			{
			}
			virtual ~task_base()
			{
			}
			virtual void run() = 0;
	};

	template<typename F> 
	class task : public task_base
	{
		public:
			task(F t):f(t)
			{
			}
			virtual void run()
			{
				return f();
			}
		private:
			F f;
	};

	class task_pool
	{
		public:
			template < typename F >
			void push(F f)
			{
				boost::shared_ptr<task_base> ptr(new(std::nothrow) task<F>(f)); 
				//task<F>* t = new (std::nothrow) task<F>(f);
				boost::recursive_mutex::scoped_lock lk(m_mtx);
				m_task.push(ptr);
			}

			boost::shared_ptr<task_base> pop()
			{
				boost::recursive_mutex::scoped_lock lk(m_mtx);
				boost::shared_ptr<task_base> task;
				while(!m_task.empty())
				{
					task = m_task.front();
					m_task.pop();
				}
				return task;
			}

			bool empty()
			{
				boost::recursive_mutex::scoped_lock  lk(m_mtx);
				m_task.empty();
			}
		private:
			boost::recursive_mutex m_mtx;
			std::queue<boost::shared_ptr<task_base> > m_task;
	};
}

#endif //_BOOST_TASK_H

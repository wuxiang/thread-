/***********************************************************************************
 * *
 * * Copyright 2011.9.15
 * * author elison.wu
 * *
 * ********************************************************************************/
#ifndef _EXTAND_THREAD_H_
#define _EXTAND_THREAD_H_
#include "extend_task.h"
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <vector>
//#include <thread>
#include <boost/shared_ptr.hpp>

namespace extend
{
	struct thread_pool_data
	{
		thread_pool_data(): m_run(true)
		{
		}
		volatile bool m_run;
		extend::task_pool m_task;
		boost::recursive_mutex m_mxt;
		boost::condition_variable_any m_cond;
	};
}

namespace extend
{
	extern "C"
	{
		inline void clean_up_routine(void* arg)
		{
			extend::thread_pool_data* pdata = static_cast<extend::thread_pool_data*>(arg);
			pdata->m_mxt.unlock();
			pdata->m_cond.notify_all();
		}

		inline void* thread_pool_routine(void* arg)
		{
			pthread_cleanup_push(clean_up_routine, arg);
			extend::thread_pool_data* pdata = static_cast<extend::thread_pool_data*>(arg);
			while(pdata->m_run)
			{
				boost::shared_ptr<task_base> ptr = pdata->m_task.pop();
				if(NULL != ptr.get())
				{
					ptr->run();
				}
				else
				{
					pdata->m_mxt.lock();
					pdata->m_cond.wait(pdata->m_mxt);
					pdata->m_mxt.unlock();
				}
			}
			pthread_cleanup_pop(0);
			return NULL;
		}
	}
}

namespace extend
{
	typedef  boost::shared_ptr<boost::thread>    thread_ptr;
	class thread_pool
	{
		public:
			thread_pool(std::size_t size)
			{
				while(size > 0)
				{
					thread_ptr  thr(new(std::nothrow) boost::thread(boost::bind(thread_pool_routine, (void*)(&m_data))));  
					m_vec.push_back(thr);
				}
			}

			~thread_pool()
			{
				this->join();
			}

			template<typename F>
			void push(F f)
			{
				m_data.m_run = false;
				m_data.m_task.push(f);
				m_data.m_cond.notify_one();
			}
			
			void notify()
			{
				m_data.m_run = false;
				m_data.m_cond.notify_all();
			}

			//void  cancel()
			//{
			//	for(std::size_t i = 0; i < m_vec.size(); ++i)
			//	{
			//		pthread_cancel(m_vec.at(i)->get_id());
			//	}
			//}

			void join()
			{
				for(std::size_t i = 0; i < m_vec.size(); ++i)
				{
					m_vec.at(i)->join();
				}
			}

			void detach()
			{
				for(std::size_t i = 0; i < m_vec.size(); ++i)
				{
					m_vec.at(i)->detach();
				}
			}

		private:
			extend::thread_pool_data m_data;
			std::size_t m_size;
			std::vector<thread_ptr>  m_vec;
	};
}

#endif //_EXTAND_THREAD_H_

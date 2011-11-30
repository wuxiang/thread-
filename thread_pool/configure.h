/***********************************************************************************
 * *
 * * Copyright 2011.9.17
 * * author elison.wu
 * *
 * ********************************************************************************/
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_
#include <iostream>
#include <boost/program_options.hpp>


const std::size_t DEFAULT_POOL_SIZE = 8;   //use for default pool size  


class  configure
{
	public:
		configure(int argc, char* argv[])
		{
			boost::program_options::options_description opt;
			boost::program_options::variables_map  vmap;
			try
			{
				opt.add_options()
					("help",  "print help for configure")
					("pool_size", 
					 	boost::program_options::value<std::size_t>(&m_pool_size)->default_value(DEFAULT_POOL_SIZE ),
					 	"set thread pool size");
				boost::program_options::store(boost::program_options::parse_command_line(argc, argv, opt), vmap);
				boost::program_options::notify(vmap);


				if(vmap.count("help"))
				{
					std::cout << opt << "\n";
					exit(0);
				}
			}
			catch(std::exception& e)
			{
				std::cout << e.what() << "\n";
				std::cout << opt << "\n";
				exit(0);
			}
			catch(...)
			{
				std::cout << "exception with unknow type" << "\n";
				std::cout << opt << "\n";
				exit(0);
			}
		}

		~configure()
		{
		}

		std::size_t get_pool_size()
		{
			return m_pool_size;
		}

		void dump_configure()
		{
			std::cout << "+++++++++++++++++++=dump_configure+++++++++++++++++++++++" << "\n";
			std::cout << "m_pool_size = " << m_pool_size << "\n";
			std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		}

	private:
		std::size_t  m_pool_size;
};

#endif //_CONFIGURE_H_

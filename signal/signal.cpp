#include <iostream>
#include <boost/signals2.hpp>
#include <boost/random.hpp>
#include <boost/bind.hpp>
char const  nurse1[] = "kate";
char const  nurse2[] = "mary";

typedef boost::variate_generator<boost::rand48, boost::uniform_smallint<> > bool_rand;
bool_rand g_rand(boost::rand48(time(0)), boost::uniform_smallint<>(0, 100));

class ring
{
	public:
		typedef boost::signals2::signal<void()> signal_t;
		typedef signal_t::slot_type  slot_t;
		boost::signals2::connection connect(slot_t p)
		{
			return alarm.connect(p);
		}
		void press()
		{
			std::cout << "alarming............" << std::endl;
			alarm();
		}
	private:
		signal_t alarm;
};

class nurse
{
	public:
		typedef void result_type;
		nurse(const std::string& name):rand(g_rand), m_name(name){}
		void action()
		{
			std::cout << m_name << std::endl;
			if(rand() > 50)
			{
				std::cout << "wake up and open the door" << std::endl;
			}
			else
			{
				std::cout << "is_sleepint..........." << std::endl;
			}
		}
	private:
		bool_rand& rand; 
		std::string m_name;
};

class guest
{
	public:
		void press(ring& r)
		{
			std::cout << "there is guest comint.........." << std::endl;
			r.press();
		}
};

int main()
{
	ring r;
	nurse nur1("kate");
	nurse nur2("hanah");
	guest g;

	boost::signals2::signal<void ()> sig;
	sig.connect(boost::bind(&nurse::action, nur1));
	sig.connect(boost::bind(&nurse::action, nur2));
	sig();
	//r.connect(ring::slot_t(boost::signals2::slot(boost::bind((&nurse::action, nur1)))));
	//r.connect(ring::slot_t(boost::signals2::slot(boost::bind((&nurse::action, nur2)))));

	g.press(r);
	return 0;
}

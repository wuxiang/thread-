#include <thread>

class scoped_thread
{
private:
    std::thread t;

public:
explicit scoped_thread(std::thread t_): t(std::move(t_))
{
    if (!t.joinable())
    {
        throw std::logic_error("NO, thread");
    }
}
~scoped_thread()
{
    t.join();
}

scoped_thread(const scoped_thread&) = delete;
scoped_thread& operator=(const scoped_thread&) = delete;

};

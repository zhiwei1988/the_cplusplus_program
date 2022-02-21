#include <iostream>
#include <thread>
#include <pthread.h>
#include <utility>

void helloFunction()
{
    std::cout << "Hello from a function." << std::endl;
}

class HelloFunctionObject {
public:
    void operator()() const 
    {
        std::cout << "Hello from a function object." << std::endl;
    }
};

class scoped_thread {
public:
    explicit scoped_thread(std::thread t_): t(std::move(t_))
    {
        if (!t.joinable()) 
            throw std::logic_error("No thread");
    }
    
    ~scoped_thread() 
    {
        t.join();
    }

    scoped_thread(scoped_thread&) = delete;
    
    scoped_thread& operator=(scoped_thread const &) = delete;
    
private:
    std::thread t;
};

int main()
{

    std::cout << std::endl;

    std::thread t1(helloFunction);

    HelloFunctionObject helloFunctionObject;
    std::thread t2(helloFunctionObject);

    std::thread t3([]{std::cout << "Hello from a lambda." << std::endl;});
    
    scoped_thread st1(std::move(t1));
    scoped_thread st2(std::move(t2));
    scoped_thread st3(std::move(t3));

    std::cout << "the end" << std::endl;
};
//
// Created by ZhiWei Tan on 2/25/22.
//

// constructing lock_guard with adopt_lock
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::lock_guard, std::adopt_lock
#include <chrono>

using namespace std;

std::mutex mtx;           // mutex for critical section
struct CriticalData{
    mutex mut;
};

void print_thread_id (int id) {
    mtx.lock();
    // std::adopt_lock 告诉 lock_guard，mutex 已经由本线程锁定，构造函数中无需对它进行加锁
    std::lock_guard<std::mutex> lck (mtx, std::adopt_lock);
    std::cout << "thread #" << id << '\n';
}

void deadLock(CriticalData& a, CriticalData& b){

    unique_lock<mutex> guard1(a.mut,defer_lock);
    cout << "Thread: " << this_thread::get_id() << " first mutex" << endl;

    this_thread::sleep_for(chrono::milliseconds(1));

    unique_lock<mutex> guard2(b.mut,defer_lock);
    cout << "  Thread: " << this_thread::get_id() << " second mutex" <<  endl;

    cout << "    Thread: " << this_thread::get_id() << " get both mutex" << endl;
    lock(guard1,guard2);
    // do something with a and b
}

int main ()
{
#if 0
    std::thread threads[10];
    // spawn 10 threads:
    for (int i=0; i<10; ++i) {
        threads[i] = std::thread(print_thread_id, i + 1);
    }

    for (auto& th : threads) {
        th.join();
    }
#endif

    CriticalData c1;
    CriticalData c2;

    thread t1([&]{deadLock(c1,c2);});
    thread t2([&]{deadLock(c2,c1);});

    t1.join();
    t2.join();

    return 0;
}
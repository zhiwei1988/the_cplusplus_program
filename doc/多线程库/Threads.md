# Threads

一个线程的创建者对其生命周期负责。创建者要么等待（t.join()），直到创建的线程 t 执行完成，要么创建者将自己从创建的线程中分离（t.detach()）。如果一个线程 t 没有被执行 t.join() 或 t.detach() 的操作，它就是可 joinable 的。一个可 joinable 的线程会在在其析构函数中调用 std::terminate，程序将会终止。如果调用了多次 t.join() or t.detach()，则会收到一个 std::system_error exception。

从创建者中分离后的线程一般被称为 daemon 线程，它在后台运行。std::thread 是一个可变参数模版，这意味着它可以接收任意数量的参数。

**如果可能，尽量不要使用 detach，因为父线程可能在子线程退出前结束，这样程序的行为是未定义**，如果不想手动管理子线程的生命周期可以使用 scoped_thread。
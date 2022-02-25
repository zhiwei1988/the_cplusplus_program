# Locks

我们应该将一个 mutex 封装在一个 lock 中，以自动释放该 mutex。lock 通过将一个 mutex 的生命周期与它自己的生命周期绑定在一起，实现了 RAII 的特性。C++ 有一个 `std::lock_guard` 用于简单的情况，还有一个`std::unique_lock/std::shared_lock` 用于复杂的使用场景，比如：显式锁定或解锁互斥。

## std::lock_guard
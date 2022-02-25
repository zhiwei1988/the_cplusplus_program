# Locks

我们应该将一个 mutex 封装在一个 lock 中，以自动释放该 mutex。lock 通过将一个 mutex 的生命周期与它自己的生命周期绑定在一起，实现了 RAII 的特性。C++ 有一个 `std::lock_guard` 用于简单的情况，还有一个`std::unique_lock/std::shared_lock` 用于复杂的使用场景，比如：显式锁定或解锁互斥。

## std::lock_guard

## std::unique_lock

**特点**

- 可以在没有关联互斥锁的情况下创建它
- 创建它而不锁定关联的互斥锁
- 显式并重复设置或释放关联互斥锁的锁
- 移动互斥锁
- 尝试锁定互斥锁
- 延迟对关联互斥锁的锁定

## std::scoped_lock

With C++17 we get a `std::scoped_lock`. It’s very similar to `std::unique_lock`, but `std::scoped_lock` can lock an arbitrary number of mutexes atomically. That being said, you have to keep two facts in mind.

## std::shared_lock

A `std::shared_lock` has the same interface as a `std::unique_lock` but behaves differently when used with a `std::shared_timed_mutex`. Many threads can share one `std::shared_timed_mutex` and, therefore, implement a reader-writer lock.
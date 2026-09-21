#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
class ThreadSafeQueue
{
private:
  std::queue<T> queue_;

  std::mutex mutex_;

  std::condition_variable cv_;

public:
  void push(T value)
  {
    {
      std::lock_guard<std::mutex> lock(mutex_);

      queue_.push(value);
    }

    cv_.notify_one();
  }

  T pop()
  {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]
             { return !queue_.empty(); });

    T value = queue_.front();
    queue_.pop();
    return value;
  }
};

ThreadSafeQueue<int> queue;

void task1()
{
  queue.push(10);
}

void task2()
{
  std::cout << queue.pop() << std::endl;
}

int main(void)
{

  std::thread t1(task1);
  std::thread t2(task2);

  t1.join();
  t2.join();
}
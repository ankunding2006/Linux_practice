#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <stdio.h>

std::queue<int> q;
std::mutex m;
std::condition_variable cv;

void producer()
{
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(43);
  }

  cv.notify_all();
}

void consumer()
{
  int value;
  {
    std::unique_lock<std::mutex> lock(m);

    cv.wait(lock, []
            { return !q.empty(); });

    value = q.front();
    q.pop();
  }

  std::cout << value << std::endl;
}

int main(void)
{
  std::thread c(consumer);
  std::thread p(producer);

  p.join();
  c.join();
}
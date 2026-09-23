#include "ThreadSafeQueue.hpp"

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
#include <iostream>
#include <thread>
#include <unistd.h>

void task1()
{
  for (uint8_t i = 0; i <= 4; ++i)
  {
    std::cout << "worker1\n";
    sleep(1);
  }
}

void task2()
{
  // sleep 0.5s
  sleep(0.5);
  for (uint8_t i = 0; i <= 4; ++i)
  {
    std::cout << "worker2\n";
    sleep(1);
  }
}

int main()
{
  std::thread t1(task1);
  std::thread t2(task2);

  t1.join();
  t2.join();

  return 0;
}
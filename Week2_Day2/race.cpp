#include <iostream>
#include <thread>

constexpr int iterations = 1000000;
int counter = 0;

void increment()
{
  for (int i = 0; i < iterations; ++i)
  {
    ++counter; // 故意保留数据竞争，用于实验
  }
}

int main()
{
  std::thread t1(increment);

  t1.join();

  std::thread t2(increment);

  t2.join();

  std::cout << "Expected: " << 2 * iterations << '\n';
  std::cout << "Actual:   " << counter << '\n';

  return 0;
}
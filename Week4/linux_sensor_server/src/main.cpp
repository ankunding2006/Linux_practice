#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <csignal>
#include <fcntl.h>
#include <errno.h>
#include <thread>
#include "main.hpp"
#include "DataQueue.hpp"

void handler(int sig)
{
  std::cout << std::endl
            << "Received signal: " << sig << std::endl;
  running = 0;
  std::cout << "running==0" << std::endl;
}

bool running = 1;
ThreadSafeQueue<data_type> queue_raw_data;
ThreadSafeQueue<data_type> queue_processor_data;

int main()
{
  std::cout
      << "begin" << std::endl;
  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  sigaddset(&action.sa_mask, SIGINT);
  action.sa_flags = 0;
  if (sigaction(SIGINT, &action, nullptr))
  {
    perror("signal");
    return 1;
  }

  std::thread t1(task_sensor_read);
  std::thread t2(task_processor);
  std::thread t3(task_server);

  t1.join();
  t2.join();
  t3.join();
}
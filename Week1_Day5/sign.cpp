#include <csignal>
#include <iostream>
#include <unistd.h>

bool running = 1;

void handler(int sig)
{
  std::cout << std::endl
            << "Received signal: " << sig << std::endl;
  running = 0;
  std::cout << "running==0" << std::endl;
}

int main()
{
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

  while (running)
  {
    sleep(1);
    std::cout << "running 1 second" << std::endl;
  }
}
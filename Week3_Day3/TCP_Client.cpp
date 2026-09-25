#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <csignal>

#define READ_BUFFER_SIZE 1024

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
  std::cout << "begin" << std::endl;
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

  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd < 0)
  {
    perror("socket");
    return 1;
  }

  struct sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8888);
  inet_pton(
      AF_INET,
      "127.0.0.1",
      &addr.sin_addr);

  if (connect(
          client_fd,
          reinterpret_cast<sockaddr *>(&addr),
          sizeof(addr)) != 0)
  {
    perror("connect");
    close(client_fd);
    return 1;
  }

  std::string msg = "hello\n";
  while (running)
  {
    ssize_t n = send(
        client_fd,
        msg.data(),
        msg.size(),
        0);

    if (n == -1)
    {
      perror("send");
      break;
    }

    sleep(1);
  }

  if (close(client_fd) == -1)
  {
    perror("close");
  }

  return 0;
}
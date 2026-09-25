#include <sys/socket.h>
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

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0)
  {
    perror("socket");
    return 1;
  }

  struct sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8888);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(
          server_fd,
          reinterpret_cast<sockaddr *>(&addr),
          sizeof(addr)) != 0)
  {
    perror("bind");
  }

  listen(server_fd, 5);

  int client_fd = accept(server_fd, nullptr, nullptr);
  if (client_fd == -1)
  {
    perror("accept");
  }

  char buffer[READ_BUFFER_SIZE];
  while (running)
  {
    ssize_t n = recv(client_fd, buffer, sizeof(buffer), 0);
    if (n > 0)
    {
      std::cout.write(buffer, n);
      std::cout.flush();
      continue;
    }

    if (n < 0)
    {
      perror("recv");
    }
    break;
  }

  if (close(client_fd) == -1)
  {
    perror("close");
  }

  return 0;
}
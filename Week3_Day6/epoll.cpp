#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <csignal>
#include <fcntl.h>
#include <errno.h>

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

  //-----------------------------------------------//

  int epfd = epoll_create1(0);

  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = server_fd;

  epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);

  struct epoll_event events[1024];

  while (running)
  {
    int num_event = epoll_wait(epfd, events, 1024, -1);
    for (int i = 0; i < num_event; i++)
    {
      int fd = events[i].data.fd;
      if (fd == server_fd)
      {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd == -1)
        {
          if (errno != EAGAIN && errno != EWOULDBLOCK)
          {
            perror("accept");
          }
          continue;
        }

        int flags = fcntl(client_fd, F_GETFL, 0);
        if (flags == -1 ||
            fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1)
        {
          perror("fcntl");
          close(client_fd);
          continue;
        }

        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = client_fd;

        epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);
      }
      else // client request
      {
        int fd = events[i].data.fd;
        char buffer[READ_BUFFER_SIZE];
        while (true) // 持续读取，直到暂时没有数据
        {
          ssize_t n = recv(fd, buffer, sizeof(buffer), 0);
          if (n > 0)
          {
            std::cout.write(buffer, n);
            std::cout.flush();

            ssize_t sent = send(fd, buffer, n, MSG_NOSIGNAL);
            if (sent < 0)
            {
              perror("send");
              break;
            }

            continue;
          }

          else if (n < 0)
          {
            if (errno == EAGAIN ||
                errno == EWOULDBLOCK)
            {
              // 当前数据已经读完，等待下一次 EPOLLIN
              break;
            }
            else
            {
              // 真正错误
              perror("recv");
              break;
            }
          }

          else // 0 已经断开
          {
            epoll_ctl(
                epfd,
                EPOLL_CTL_DEL,
                fd,
                nullptr);
            if (close(fd) == -1)
            {
              perror("close");
            }
            break;
          }
        }
      }
    }
  }
  return 0;
}

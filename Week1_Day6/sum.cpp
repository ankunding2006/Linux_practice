/*
完成一个小程序：

父进程
↓
fork
↓
子进程执行一个任务
↓
父进程等待
↓
捕获 Ctrl+C
↓
安全结束
*/
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

void handler(int sig);

int main(void)
{
  pid_t result = fork();
  if (result < 0)
  {
    perror("fork 失败");
    return 1;
  }
  else if (result == 0)
  {
    // child
    printf("child 进程\n");
    std::cout << "这是子进程: PID == " << getpid() << std::endl;
    char *argv[] = {
        const_cast<char *>("ls"),
        const_cast<char *>("-l"),
        nullptr};
    execvp(argv[0], argv);
    execvp("ls", argv);
    perror("execvp");
    exit(1);
  }
  else
  {
    // parent
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
    int status;
    pid_t PID_wait = waitpid(result, &status, 0);
    if (PID_wait == -1)
    {
      perror("waitpid");
      exit(1);
    }
    else
    {
      printf("子进程PID:%d\n", PID_wait);
    }
  }
}

void handler(int sig)
{
  std::cout << std::endl
            << "Received signal: " << sig << std::endl;
}
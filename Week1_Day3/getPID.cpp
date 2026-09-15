#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
  int number = 10;

  printf("fork 前：PID = %d，number = %d\n", static_cast<int>(getpid()), number);
  fflush(stdout);

  pid_t result = fork();

  if (result < 0)
  {
    perror("fork 失败");
    return 1;
  }

  if (result == 0)
  {
    number = 20;
    printf("子进程：PID = %d，PPID = %d，number = %d\n",
           static_cast<int>(getpid()), static_cast<int>(getppid()), number);
  }
  else
  {
    number = 30;
    printf("父进程：PID = %d，子进程 PID = %d，number = %d\n",
           static_cast<int>(getpid()), static_cast<int>(result), number);

    waitpid(result, nullptr, 0);
    printf("父进程：已经等到子进程结束\n");
  }

  return 0;
}
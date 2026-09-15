#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
  pid_t PID;
  int local = 10;
  printf("before\n");

  PID = fork();
  if (PID > 0)
  {
    printf("这是父进程:PID==%d,local==%d\n", getpid(), local);
    local = 100;
    printf("这是父进程:PID==%d,local==%d\n", getpid(), local);
    int status;
    pid_t PID_wait = waitpid(PID, &status, 0);
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

  else if (PID == 0)
  {
    printf("这是子进程:PID==%d,local==%d\n", getpid(), local);
    local = 30;
    printf("这是子进程:PID==%d,local==%d\n", getpid(), local);
    char *const argv[] = {"ls", "-l", nullptr};
    execvp("ls", argv);
    perror("execvp");
    exit(1);
  }

  else //-1
  {
    perror("fork");
    return -1;
  }
  return 0;
}
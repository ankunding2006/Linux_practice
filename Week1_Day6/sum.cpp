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

void handler(int sig);

int main(void)
{
}

void handler(int sig)
{
  printf("Ctrl+C\n");
}
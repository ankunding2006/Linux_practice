/*
 * print.c
 * 实现功能:
 *
 * 打开 a.txt
 * 读取内容
 * 输出到终端
 * 关闭文件
 *
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BYTE_TO_READ 1024

int main(void)
{
  int fd;
  char buf[BYTE_TO_READ];
  ssize_t n;

  fd = open("a.txt", O_RDONLY);

  if (fd == -1)
  {
    perror("open");
    return -1;
  }

  n = read(fd, buf, BYTE_TO_READ);

  printf("读取到的字节数: %zd\n", n);

  if (n == -1)
  {
    perror("read");
    close(fd);
    return -1;
  }

  write(STDOUT_FILENO, buf, n);
  printf("\n");

  close(fd);

  return 0;
}
/* 循环读文件(把整个文件读完和读满 N 字节) */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <time.h>

int Whole_read(void);
int Read_N(int Number_to_read);

int main()
{
  // Whole_read();
  Read_N(257);
}

int Read_N(int Number_to_read)
{
  if (Number_to_read <= 0 || Number_to_read > 1024)
  {
    return -1;
  }

  int fd;
  int TotalNumber_have_read = 0; // 已经读取到的字节数
  ssize_t ThisTime_number_have_read;
  char buf[1024];

  fd = open("./源文件.txt", O_RDONLY);
  if (fd == -1)
  {
    perror("open");
    return -1;
  }
  while (Number_to_read > TotalNumber_have_read)
  {
    ThisTime_number_have_read = read(fd, buf + TotalNumber_have_read, Number_to_read - TotalNumber_have_read);
    if (ThisTime_number_have_read == -1)
    {
      if (errno == EINTR)
      {
        continue;
      }

      perror("read");
      close(fd);
      return -1;
    }
    else if (ThisTime_number_have_read > 0)
    {
      TotalNumber_have_read = ThisTime_number_have_read + TotalNumber_have_read;
    }
    else
    {
      // 没有数据时继续等待，避免循环占满 CPU
      struct timespec delay = {0, 10 * 1000 * 1000};
      nanosleep(&delay, nullptr);
      continue;
    }
  }
  std::cout << "文件内容:";
  std::cout.write(buf, TotalNumber_have_read);
  std::cout << std::endl;
  close(fd);
  return 0;
}

int Whole_read()
{
  int fd;
  int n = 0;
  int total = 0;
  char buf[1024];
  fd = open("./源文件.txt", O_RDONLY);
  if (fd == -1)
  {
    perror("open");
    return -1;
  }

  // 打开完毕开始读取
  while (1)
  {
    n = read(fd, buf, sizeof(buf));

    if (n > 0)
    {
      // 处理
      total = total + n;
      std::cout << "文件内容:";
      std::cout.write(buf, n);
      std::cout << std::endl
                << "total:" << total;
      std::cout << std::endl;
    }
    else if (n == 0)
    {
      // 到达EOF
      break;
    }
    else
    {
      perror("read");
      close(fd);
      return -1;
    }
  }
  close(fd);
  return 0;
}
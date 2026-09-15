/*
实现：

./mycp source.txt destination.txt

要求：

- 文件不存在能够正确提示
- 正确循环 read
- 正确处理部分 write
- 正确关闭 FD
*/

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <time.h>

int main(void)
{
  ssize_t fd1;
  ssize_t fd2;
  long int Total_char = 0, Char_have_write = 0;
  ssize_t Num_read;
  long int Offset = 0;
  std::vector<char> buf(2048);
  fd1 = open("./源文件.txt", O_RDONLY);
  while (1)
  {

    Num_read = read(fd1, buf.data() + Offset, buf.size() - Offset);
    if (Num_read > 0)
    {
      Offset += Num_read;
      if (Offset == 2048)
      {
        std::cout << "数组已满";
      }
    }
    else if (Num_read == 0)
    {
      Total_char = Offset;
      break;
    }
    else
    {
      close(fd1);
      return -1;
    }
  }
  close(fd1);
  fd2 = open("./目标文件.txt", O_WRONLY | O_CREAT, S_IRWXU);
  if (fd2 == -1)
  {
    std::cout << "打开文件错误";
    perror("open");
  }
  while (Total_char > Char_have_write)
  {
    long int Char_this_write = 0;
    Char_this_write = write(fd2, buf.data() + Char_have_write, Total_char - Char_have_write);
    if (Char_this_write > 0)
    {
      Char_have_write += Char_this_write;
    }
    else if (Char_this_write < 0)
    {
      perror("write");
      close(fd2);
      return -1;
    }
    else //==0
    {
      std::cout << "写入错误";
      close(fd2);
      return -1;
    }
  }
  close(fd2);
  return 1;
}
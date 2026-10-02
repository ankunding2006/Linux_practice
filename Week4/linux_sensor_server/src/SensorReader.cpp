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
#include "DataQueue.hpp"
#include "main.hpp"

void task_sensor_read()
{
  ssize_t Num_read;
  long int Offset = 0;
  std::vector<char> buf(2048);
  ssize_t fd1;
  fd1 = open("/root/Desktop/练习/Week4/linux_sensor_server/sensor/sensor_data", O_RDONLY);
  while (running)
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
      lseek(fd1, 0, SEEK_SET);
      Offset = 0;
      std::fill(buf.begin(), buf.end(), 0);
    }
    else
    {
      close(fd1);
      perror("read_sensor");
      running = 0;
    }

    queue_raw_data.push(buf[0]);
  }
  close(fd1);
}
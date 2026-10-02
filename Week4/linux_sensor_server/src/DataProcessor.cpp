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
void pocess_data(data_type &data)
{
  // ¥¶¿Ì
}

void task_processor()
{
  while (running)
  {
    if (!queue_raw_data.empty())
    {
      data_type data = queue_raw_data.pop();
      pocess_data(data);
      queue_processor_data.push(data);
    }
  }
}
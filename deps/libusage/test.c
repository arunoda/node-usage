#include <stdio.h>
#include "usage.h"

int main(int argc, char const *argv[]) {

  int pid = atoi(argv[1]);
  usage_t* usage_info = usage_init(pid);

  if(usage_info) {
    float cpu_usage = usage_cpu(usage_info);
    long mem = usage_totalmem(usage_info);

    printf("CPU: %.2f, MEM: %lu\n", cpu_usage, mem);
    usage_clean(usage_info);
  } else {
    printf("No Such PID\n");
  }

	return 0;
}
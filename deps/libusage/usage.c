#include "usage.h"

usage_t* usage_init(int pid) {
  
  proc_t * proc_info = malloc(sizeof(proc_t));
  if(get_proc_stats(pid, proc_info)) {
    return proc_info;
  } else {
    free(proc_info);
    return NULL;
  }
}

void usage_clean(usage_t* usage_info) {
  
  freeproc(usage_info);
}

float usage_cpu(usage_t* pp){
 
  unsigned long long total_time;   /* jiffies used by this process */
  unsigned pcpu = 0;               /* scaled %cpu, 999 means 99.9% */
  unsigned long long seconds;      /* seconds of process life */
  unsigned int seconds_since_boot = uptime(0, 0);

  total_time = pp->utime + pp->stime;
  seconds = seconds_since_boot - pp->start_time / Hertz;
  if(seconds) pcpu = (total_time * 1000ULL / Hertz) / seconds;
  return (float) pcpu/10U;
}

unsigned long usage_totalmem(usage_t* pp) {

  long page_size = sysconf(_SC_PAGESIZE);
  return pp->rss * page_size;
}


int abc() {
  return 2000;
}
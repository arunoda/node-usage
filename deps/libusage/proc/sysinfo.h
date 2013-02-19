#ifndef PROC_SYSINFO_H
#define PROC_SYSINFO_H
#include <sys/types.h>
#include <sys/dir.h>
#include "procps.h"

EXTERN_C_BEGIN

extern unsigned long long Hertz;   /* clock tick frequency */
extern long smp_num_cpus;          /* number of CPUs */
extern int have_privs;             /* boolean, true if setuid or similar */

#if 0
#define JT double
extern void eight_cpu_numbers(JT *uret, JT *nret, JT *sret, JT *iret, JT *wret, JT *xret, JT *yret, JT *zret);
#undef JT
#endif

extern int        uptime (double *uptime_secs, double *idle_secs);
extern unsigned long getbtime(void);
extern void       loadavg(double *av1, double *av5, double *av15);


/* obsolete */
extern unsigned long kb_main_shared;
/* old but still kicking -- the important stuff */
extern unsigned long kb_main_buffers;
extern unsigned long kb_main_cached;
extern unsigned long kb_main_free;
extern unsigned long kb_main_total;
extern unsigned long kb_swap_free;
extern unsigned long kb_swap_total;
/* recently introduced */
extern unsigned long kb_high_free;
extern unsigned long kb_high_total;
extern unsigned long kb_low_free;
extern unsigned long kb_low_total;
/* 2.4.xx era */
extern unsigned long kb_active;
extern unsigned long kb_inact_laundry;  // grrr...
extern unsigned long kb_inact_dirty;
extern unsigned long kb_inact_clean;
extern unsigned long kb_inact_target;
extern unsigned long kb_swap_cached;  /* late 2.4+ */
/* derived values */
extern unsigned long kb_swap_used;
extern unsigned long kb_main_used;
/* 2.5.41+ */
extern unsigned long kb_writeback;
extern unsigned long kb_slab;
extern unsigned long nr_reversemaps;
extern unsigned long kb_committed_as;
extern unsigned long kb_dirty;
extern unsigned long kb_inactive;
extern unsigned long kb_mapped;
extern unsigned long kb_pagetables;

#define BUFFSIZE (64*1024)
typedef unsigned long long jiff;
extern void getstat(jiff *__restrict cuse, jiff *__restrict cice, jiff *__restrict csys, jiff *__restrict cide, jiff *__restrict ciow, jiff *__restrict cxxx, jiff *__restrict cyyy, jiff *__restrict czzz,
	     unsigned long *__restrict pin, unsigned long *__restrict pout, unsigned long *__restrict s_in, unsigned long *__restrict sout,
	     unsigned *__restrict intr, unsigned *__restrict ctxt,
	     unsigned int *__restrict running, unsigned int *__restrict blocked,
	     unsigned int *__restrict btime, unsigned int *__restrict processes);

extern void meminfo(void);


extern unsigned long vm_nr_dirty;
extern unsigned long vm_nr_writeback;
extern unsigned long vm_nr_pagecache;
extern unsigned long vm_nr_page_table_pages;
extern unsigned long vm_nr_reverse_maps;
extern unsigned long vm_nr_mapped;
extern unsigned long vm_nr_slab;
extern unsigned long vm_pgpgin;
extern unsigned long vm_pgpgout;
extern unsigned long vm_pswpin;
extern unsigned long vm_pswpout;
extern unsigned long vm_pgalloc;
extern unsigned long vm_pgfree;
extern unsigned long vm_pgactivate;
extern unsigned long vm_pgdeactivate;
extern unsigned long vm_pgfault;
extern unsigned long vm_pgmajfault;
extern unsigned long vm_pgscan;
extern unsigned long vm_pgrefill;
extern unsigned long vm_pgsteal;
extern unsigned long vm_kswapd_steal;
extern unsigned long vm_pageoutrun;
extern unsigned long vm_allocstall;

extern void vminfo(void);

typedef struct disk_stat{
	unsigned long long reads_sectors;
	unsigned long long written_sectors;
	char               disk_name [16];
	unsigned           inprogress_IO;
	unsigned           merged_reads;
	unsigned           merged_writes;
	unsigned           milli_reading;
	unsigned           milli_spent_IO;
	unsigned           milli_writing;
	unsigned           partitions;
	unsigned           reads;
	unsigned           weighted_milli_spent_IO;
	unsigned           writes;
}disk_stat;

typedef struct partition_stat{
	char partition_name [16];
	unsigned long long reads_sectors;
	unsigned           parent_disk;  // index into a struct disk_stat array
	unsigned           reads;
	unsigned           writes;
	unsigned long long requested_writes;
}partition_stat;

extern unsigned int getpartitions_num(struct disk_stat *disks, int ndisks);
extern unsigned int getdiskstat (struct disk_stat**,struct partition_stat**);

typedef struct slab_cache{
	char name[48];
	unsigned active_objs;
	unsigned num_objs;
	unsigned objsize;
	unsigned objperslab;
}slab_cache;

extern unsigned int getslabinfo (struct slab_cache**);

extern unsigned get_pid_digits(void) FUNCTION;

extern void cpuinfo (void);

EXTERN_C_END
#endif /* SYSINFO_H */

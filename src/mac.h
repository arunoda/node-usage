#ifndef MAC_H
#define MAC_H

#include <stdlib.h>
#include <string.h>
#include <node.h>
#include <uv.h>

typedef struct ps_usage {
    double cpu;
    long memory;
} ps_usage_t;

using namespace v8;
struct UsageData {

    int pid;
    Persistent<Function> callback;
    ps_usage_t ps_usage;
    int res;
};

extern int get_usage(int pid, ps_usage_t* ps_usage);
extern Handle<Value> GetUsage(const Arguments& args);

#endif
